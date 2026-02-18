#define TB_IMPL
#define TB_PLATFORM_WASM

#include <stdlib.h>

typedef long ssize_t;

#include "tb_api.h"
#include "tb_platform.h"

/* Host-provided imports (implemented by JavaScript runtime) */
extern int __tb_host_write(const char *buf, int len);
extern int __tb_host_read(char *buf, int len);
extern int __tb_host_wait(int timeout_ms); /* returns bitmask: 1=input, 2=resize */
extern void __tb_host_get_size(int *w, int *h);

struct tb_wasm {
    int resized;
};

void *tb_platform_open(const char *path, int *err) {
    (void)path;
    struct tb_wasm *p = tb_malloc(sizeof(struct tb_wasm));
    if (!p) { *err = TB_ERR_MEM; return NULL; }
    memset(p, 0, sizeof(*p));
    return p;
}

void *tb_platform_open_fd(int fd, int *err) {
    (void)fd;
    return tb_platform_open("", err);
}

void *tb_platform_open_rwfd(int rfd, int wfd, int *err) {
    (void)rfd;
    (void)wfd;
    return tb_platform_open("", err);
}

int tb_platform_enter_raw(void *platform) {
    (void)platform;
    return TB_OK;
}

int tb_platform_write(void *platform, const char *buf, size_t len) {
    (void)platform;
    int rv = __tb_host_write(buf, (int)len);
    if (rv < 0) return TB_ERR;
    return TB_OK;
}

int tb_platform_read(void *platform, char *buf, size_t len,
        ssize_t *nread) {
    (void)platform;
    int rv = __tb_host_read(buf, (int)len);
    if (rv < 0) {
        *nread = 0;
        return TB_ERR;
    }
    *nread = rv;
    return TB_OK;
}

int tb_platform_init_resize(void *platform) {
    (void)platform;
    return TB_OK;
}

int tb_platform_wait(void *platform, int timeout_ms, int *has_input,
        int *has_resize) {
    (void)platform;
    int rv = __tb_host_wait(timeout_ms);
    if (rv < 0) {
        *has_input = 0;
        *has_resize = 0;
        return -1;
    } else if (rv == 0) {
        *has_input = 0;
        *has_resize = 0;
        return 0;
    }
    *has_input = (rv & 1) ? 1 : 0;
    *has_resize = (rv & 2) ? 1 : 0;
    return 1;
}

int tb_platform_drain_resize(void *platform) {
    (void)platform;
    return TB_OK;
}

int tb_platform_term_size(void *platform, int *w, int *h) {
    (void)platform;
    __tb_host_get_size(w, h);
    return TB_OK;
}

int tb_platform_init_caps(void *platform) {
    (void)platform;
    int j;
    for (j = 0; j < TB_CAP__COUNT; j++) {
        global.caps[j] = builtin_terms[0].caps[j];
    }
    return TB_OK;
}

int tb_platform_can_write(void *platform) {
    (void)platform;
    return 1;
}

int tb_platform_get_fds(void *platform, int *ttyfd, int *resizefd) {
    (void)platform;
    *ttyfd = -1;
    *resizefd = -1;
    return TB_OK;
}

void tb_platform_shutdown(void *platform) {
    if (platform) tb_free(platform);
}
