#define TB_IMPL

#include "tb_api.h"
#include "tb_platform.h"

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>

struct tb_posix {
    int ttyfd;
    int rfd;
    int wfd;
    int ttyfd_open;
    int resize_pipefd[2];
    struct termios orig_tios;
    int has_orig_tios;
    char *terminfo;
    size_t nterminfo;
};

static struct tb_posix *tb_posix_alloc(void) {
    struct tb_posix *p = tb_malloc(sizeof(struct tb_posix));
    if (!p) return NULL;
    memset(p, 0, sizeof(*p));
    p->ttyfd = -1;
    p->rfd = -1;
    p->wfd = -1;
    p->resize_pipefd[0] = -1;
    p->resize_pipefd[1] = -1;
    return p;
}

void *tb_platform_open(const char *path, int *err) {
    struct tb_posix *p = tb_posix_alloc();
    if (!p) { *err = TB_ERR_MEM; return NULL; }

    int ttyfd = open(path, O_RDWR);
    if (ttyfd < 0) {
        global.last_errno = errno;
        tb_free(p);
        *err = TB_ERR_INIT_OPEN;
        return NULL;
    }
    p->ttyfd_open = 1;
    p->ttyfd = ttyfd;
    p->rfd = ttyfd;
    p->wfd = ttyfd;
    return p;
}

void *tb_platform_open_fd(int fd, int *err) {
    struct tb_posix *p = tb_posix_alloc();
    if (!p) { *err = TB_ERR_MEM; return NULL; }
    p->rfd = fd;
    p->wfd = fd;
    (void)err;
    return p;
}

void *tb_platform_open_rwfd(int rfd, int wfd, int *err) {
    struct tb_posix *p = tb_posix_alloc();
    if (!p) { *err = TB_ERR_MEM; return NULL; }
    p->rfd = rfd;
    p->wfd = wfd;
    (void)err;
    return p;
}

int tb_platform_enter_raw(void *platform) {
    struct tb_posix *p = platform;
    if (p->ttyfd < 0) return TB_OK;

    if (tcgetattr(p->ttyfd, &p->orig_tios) != 0) {
        global.last_errno = errno;
        return TB_ERR_TCGETATTR;
    }

    struct termios tios;
    memcpy(&tios, &p->orig_tios, sizeof(tios));
    p->has_orig_tios = 1;

    cfmakeraw(&tios);
    tios.c_cc[VMIN] = 1;
    tios.c_cc[VTIME] = 0;

    if (tcsetattr(p->ttyfd, TCSAFLUSH, &tios) != 0) {
        global.last_errno = errno;
        return TB_ERR_TCSETATTR;
    }

    return TB_OK;
}

int tb_platform_write(void *platform, const char *buf, size_t len) {
    struct tb_posix *p = platform;
    ssize_t rv = write(p->wfd, buf, len);
    if (rv < 0 || (size_t)rv != len) {
        global.last_errno = errno;
        return TB_ERR;
    }
    return TB_OK;
}

int tb_platform_read(void *platform, char *buf, size_t len,
        ssize_t *nread) {
    struct tb_posix *p = platform;
    ssize_t rv = read(p->rfd, buf, len);
    if (rv < 0) {
        global.last_errno = errno;
        *nread = 0;
        return TB_ERR;
    }
    *nread = rv;
    return TB_OK;
}

int tb_platform_wait(void *platform, int timeout_ms, int *has_input,
        int *has_resize) {
    struct tb_posix *p = platform;
    fd_set fds;
    struct timeval tv;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms - (tv.tv_sec * 1000)) * 1000;

    FD_ZERO(&fds);
    FD_SET(p->rfd, &fds);
    FD_SET(p->resize_pipefd[0], &fds);

    int maxfd = p->resize_pipefd[0] > p->rfd
                    ? p->resize_pipefd[0]
                    : p->rfd;

    int rv =
        select(maxfd + 1, &fds, NULL, NULL, (timeout_ms < 0) ? NULL : &tv);

    if (rv < 0) {
        global.last_errno = errno;
        *has_input = 0;
        *has_resize = 0;
        return -1;
    } else if (rv == 0) {
        *has_input = 0;
        *has_resize = 0;
        return 0;
    }

    *has_input = FD_ISSET(p->rfd, &fds) ? 1 : 0;
    *has_resize = FD_ISSET(p->resize_pipefd[0], &fds) ? 1 : 0;
    return 1;
}

int tb_platform_drain_resize(void *platform) {
    struct tb_posix *p = platform;
    int ignore = 0;
    read(p->resize_pipefd[0], &ignore, sizeof(ignore));
    return TB_OK;
}

static int tb_platform_term_size_via_esc(struct tb_posix *p, int *w, int *h) {
#ifndef TB_RESIZE_FALLBACK_MS
#define TB_RESIZE_FALLBACK_MS 1000
#endif

    char move_and_report[] = "\x1b[9999;9999H\x1b[6n";
    ssize_t write_rv =
        write(p->wfd, move_and_report, strlen(move_and_report));
    if (write_rv != (ssize_t)strlen(move_and_report)) {
        return TB_ERR_RESIZE_WRITE;
    }

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(p->rfd, &fds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = TB_RESIZE_FALLBACK_MS * 1000;

    int select_rv = select(p->rfd + 1, &fds, NULL, NULL, &timeout);

    if (select_rv != 1) {
        global.last_errno = errno;
        return TB_ERR_RESIZE_POLL;
    }

    char buf[TB_OPT_READ_BUF];
    ssize_t nread = 0;
    if (tb_platform_read(p, buf, sizeof(buf) - 1, &nread) != TB_OK
            || nread < 1) {
        return TB_ERR_RESIZE_READ;
    }
    buf[nread] = '\0';

    int rw, rh;
    if (sscanf(buf, "\x1b[%d;%dR", &rh, &rw) != 2) {
        return TB_ERR_RESIZE_SSCANF;
    }

    *w = rw;
    *h = rh;
    return TB_OK;
}

void tb_platform_shutdown(void *platform) {
    struct tb_posix *p = platform;
    if (!p) return;

    if (p->ttyfd >= 0) {
        if (p->has_orig_tios) {
            tcsetattr(p->ttyfd, TCSAFLUSH, &p->orig_tios);
        }
        if (p->ttyfd_open) {
            close(p->ttyfd);
        }
    }

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = SIG_DFL;
    sigaction(SIGWINCH, &sa, NULL);

    if (p->resize_pipefd[0] >= 0) close(p->resize_pipefd[0]);
    if (p->resize_pipefd[1] >= 0) close(p->resize_pipefd[1]);

    if (p->terminfo) tb_free(p->terminfo);

    tb_free(p);
}

int tb_platform_can_write(void *platform) {
    struct tb_posix *p = platform;
    return p->wfd >= 0;
}

int tb_platform_get_fds(void *platform, int *ttyfd, int *resizefd) {
    struct tb_posix *p = platform;
    *ttyfd = p->rfd;
    *resizefd = p->resize_pipefd[0];
    return TB_OK;
}

static int get_terminfo_int16(struct tb_posix *p, int offset, int16_t *val) {
    if (offset < 0 || offset + (int)sizeof(int16_t) > (int)p->nterminfo) {
        *val = -1;
        return TB_ERR;
    }
    memcpy(val, p->terminfo + offset, sizeof(int16_t));
    return TB_OK;
}

static const char *get_terminfo_string(struct tb_posix *p,
        int16_t offsets_pos, int16_t offsets_len,
        int16_t table_pos, int16_t table_size, int16_t index) {
    if (index >= offsets_len) {
        return "";
    }

    int16_t table_offset;
    int table_offset_offset = (int)offsets_pos + (index * (int)sizeof(int16_t));
    if (get_terminfo_int16(p, table_offset_offset, &table_offset) != TB_OK) {
        return NULL;
    }

    if (table_offset < 0 || table_offset >= table_size) {
        return "";
    }

    int str_offset = (int)table_pos + (int)table_offset;
    if (str_offset >= (int)p->nterminfo) {
        return NULL;
    }

    return (const char *)(p->terminfo + str_offset);
}

static int read_terminfo_path(struct tb_posix *p, const char *path) {
    FILE *fp = fopen(path, "rb");
    if (!fp) return TB_ERR;

    struct stat st;
    if (fstat(fileno(fp), &st) != 0) {
        fclose(fp);
        return TB_ERR;
    }

    size_t fsize = st.st_size;
    char *data = (char *)tb_malloc(fsize);
    if (!data) {
        fclose(fp);
        return TB_ERR;
    }

    if (fread(data, 1, fsize, fp) != fsize) {
        fclose(fp);
        tb_free(data);
        return TB_ERR;
    }

    p->terminfo = data;
    p->nterminfo = fsize;

    fclose(fp);
    return TB_OK;
}

static int load_terminfo_from_path(struct tb_posix *p, const char *path,
        const char *term) {
    int rv;
    char tmp[TB_PATH_MAX];

    snprintf_or_return(rv, tmp, sizeof(tmp), "%s/%c/%s", path, term[0], term);
    if_ok_return(rv, read_terminfo_path(p, tmp));

#ifdef __APPLE__
    snprintf_or_return(rv, tmp, sizeof(tmp), "%s/%x/%s", path, term[0], term);
    return read_terminfo_path(p, tmp);
#endif

    return TB_ERR;
}

static int load_terminfo(struct tb_posix *p) {
    int rv;
    char tmp[TB_PATH_MAX];

    const char *term = getenv("TERM");
    if (!term) return TB_ERR;

    const char *terminfo = getenv("TERMINFO");
    if (terminfo) if_ok_return(rv, load_terminfo_from_path(p, terminfo, term));

    const char *home = getenv("HOME");
    if (home) {
        snprintf_or_return(rv, tmp, sizeof(tmp), "%s/.terminfo", home);
        if_ok_return(rv, load_terminfo_from_path(p, tmp, term));
    }

    const char *dirs = getenv("TERMINFO_DIRS");
    if (dirs) {
        snprintf_or_return(rv, tmp, sizeof(tmp), "%s", dirs);
        char *dir = strtok(tmp, ":");
        while (dir) {
            const char *cdir = dir;
            if (*cdir != '\0') {
                if_ok_return(rv, load_terminfo_from_path(p, cdir, term));
            }
            dir = strtok(NULL, ":");
        }
    }

#ifdef TB_TERMINFO_DIR
    if_ok_return(rv, load_terminfo_from_path(p, TB_TERMINFO_DIR, term));
#endif
    if_ok_return(rv, load_terminfo_from_path(p, "/usr/local/etc/terminfo", term));
    if_ok_return(rv,
        load_terminfo_from_path(p, "/usr/local/share/terminfo", term));
    if_ok_return(rv, load_terminfo_from_path(p, "/usr/local/lib/terminfo", term));
    if_ok_return(rv, load_terminfo_from_path(p, "/etc/terminfo", term));
    if_ok_return(rv, load_terminfo_from_path(p, "/usr/share/terminfo", term));
    if_ok_return(rv, load_terminfo_from_path(p, "/usr/lib/terminfo", term));
    if_ok_return(rv, load_terminfo_from_path(p, "/usr/share/lib/terminfo", term));
    if_ok_return(rv, load_terminfo_from_path(p, "/lib/terminfo", term));

    return TB_ERR;
}

static int parse_terminfo_caps(struct tb_posix *p) {
    if (p->nterminfo < 6 * (int)sizeof(int16_t)) return TB_ERR;

    int16_t magic_number, nbytes_names, nbytes_bools, num_ints, num_offsets,
        nbytes_strings;
    size_t nbytes_header = 6 * sizeof(int16_t);

    get_terminfo_int16(p, 0 * sizeof(int16_t), &magic_number);
    get_terminfo_int16(p, 1 * sizeof(int16_t), &nbytes_names);
    get_terminfo_int16(p, 2 * sizeof(int16_t), &nbytes_bools);
    get_terminfo_int16(p, 3 * sizeof(int16_t), &num_ints);
    get_terminfo_int16(p, 4 * sizeof(int16_t), &num_offsets);
    get_terminfo_int16(p, 5 * sizeof(int16_t), &nbytes_strings);

    const int bytes_per_int = magic_number == 01036 ? 4 : 2;
    const int align_offset = (nbytes_names + nbytes_bools) % 2 != 0 ? 1 : 0;

    const int pos_str_offsets =
        nbytes_header + nbytes_names + nbytes_bools + align_offset +
        (num_ints * bytes_per_int);

    const int pos_str_table =
        pos_str_offsets + (num_offsets * sizeof(int16_t));

    int i;
    for (i = 0; i < TB_CAP__COUNT; i++) {
        const char *cap = get_terminfo_string(p, pos_str_offsets, num_offsets,
            pos_str_table, nbytes_strings, terminfo_cap_indexes[i]);
        if (!cap) {
            return TB_ERR;
        }
        global.caps[i] = cap;
    }

    return TB_OK;
}

static int load_builtin_caps(void) {
    int i, j;
    const char *term = getenv("TERM");

    if (!term) return TB_ERR_NO_TERM;

    for (i = 0; builtin_terms[i].name != NULL; i++) {
        if (strcmp(term, builtin_terms[i].name) == 0) {
            for (j = 0; j < TB_CAP__COUNT; j++) {
                global.caps[j] = builtin_terms[i].caps[j];
            }
            return TB_OK;
        }
    }

    for (i = 0; builtin_terms[i].name != NULL; i++) {
        if (strstr(term, builtin_terms[i].name) != NULL ||
            (*(builtin_terms[i].alias) != '\0' &&
                strstr(term, builtin_terms[i].alias) != NULL))
        {
            for (j = 0; j < TB_CAP__COUNT; j++) {
                global.caps[j] = builtin_terms[i].caps[j];
            }
            return TB_OK;
        }
    }

    return TB_ERR_UNSUPPORTED_TERM;
}

static void tb_platform_handle_resize(int sig) {
    int errno_copy = errno;
    struct tb_posix *p = global.platform;
    write(p->resize_pipefd[1], &sig, sizeof(sig));
    errno = errno_copy;
}

int tb_platform_init(void *platform) {
    struct tb_posix *p = platform;

    if (pipe(p->resize_pipefd) != 0) {
        global.last_errno = errno;
        return TB_ERR_RESIZE_PIPE;
    }

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = tb_platform_handle_resize;
    if (sigaction(SIGWINCH, &sa, NULL) != 0) {
        global.last_errno = errno;
        return TB_ERR_RESIZE_SIGACTION;
    }

    if (load_terminfo(p) == TB_OK) {
        return parse_terminfo_caps(p);
    }
    return load_builtin_caps();
}

int tb_platform_term_size(void *platform, int *w, int *h) {
    struct tb_posix *p = platform;
    int ioctl_errno;

    if (p->ttyfd < 0) return TB_OK;

    struct winsize sz;
    memset(&sz, 0, sizeof(sz));

    if (ioctl(p->ttyfd, TIOCGWINSZ, &sz) == 0) {
        *w = sz.ws_col;
        *h = sz.ws_row;
        return TB_OK;
    }
    ioctl_errno = errno;

    if (tb_platform_term_size_via_esc(p, w, h) == TB_OK) {
        return TB_OK;
    }

    global.last_errno = ioctl_errno;
    return TB_ERR_RESIZE_IOCTL;
}
