#!/bin/bash
set -euo pipefail

main() {
    init_files "$@"
    alter_headers
    alter_structs
    alter_errors
    alter_functions
}

init_files() {
    in=${1:-termbox2.h}
    out=${1:-termbox2_win.h}
    cp -vf "$in" "$out"
    tmp=$(mktemp)
}

alter_headers() {
    my_sed '/__posix_header_start/,/__posix_header_end/c\#include <windows.h>\n#include <synchapi.h>\n'
}

alter_structs() {
    my_sed 's/^(\s+)int (rfd|wfd|ttyfd);/\1HANDLE \2;/g'
    my_sed 's/^(\s+)struct termios orig_tios;/\1DWORD orig_tios[2];/g'
}

alter_errors() {
    local err_line
    local err_line_num
    local err_val
    err_line=$(grep -E -n '^#define TB_ERR_.+[0-9]+$' "$out" | tail -n1)
    err_line_num=$(cut -d: -f1 <<<"$err_line")
    err_val=$(awk '{print $NF}' <<<"$err_line")
    my_sed "${err_line_num}a #define TB_ERR_WIN_UNSUPPORTED $((err_val - 5))"
    my_sed "${err_line_num}a #define TB_ERR_WIN_NO_STDIO    $((err_val - 4))"
    my_sed "${err_line_num}a #define TB_ERR_WIN_SET_CONMODE $((err_val - 3))"
    my_sed "${err_line_num}a #define TB_ERR_WIN_GET_CONMODE $((err_val - 2))"
    my_sed "${err_line_num}a #define TB_ERR_WIN_RESIZE      $((err_val - 1))"
    my_sed '/case TB_ERR:/i \        case TB_ERR_WIN_UNSUPPORTED: return "Unsupporrted (Windows)";'
    my_sed '/case TB_ERR:/i \        case TB_ERR_WIN_NO_STDIO:    return "Stdio not available (Windows)";'
    my_sed '/case TB_ERR:/i \        case TB_ERR_WIN_SET_CONMODE: return "Failed to set console mode (Windows)";'
    my_sed '/case TB_ERR:/i \        case TB_ERR_WIN_GET_CONMODE: return "Failed to get console mode (Windows)";'
    my_sed '/case TB_ERR:/i \        case TB_ERR_WIN_RESIZE:      return "Failed to resize console (Windows)";'
}

alter_functions() {
    replace_function 'int tb_get_fds'                      <<<'    return TB_ERR_WIN_UNSUPPORTED;'
    replace_function 'static int update_term_size_via_esc' <<<'    return TB_ERR;'
    replace_function 'static int load_terminfo'            <<<'    return TB_ERR;'
    replace_function 'static void handle_resize'           <<<'    (void)sig;';
    replace_function 'static int init_resize_handler'      <<<'    return TB_OK;'

    my_sed 's/getenv\("TERM"\)/"xterm-256color"/'

    alter_function 'const char \*tb_strerror' <<'EOD'
            snprintf(global.errbuf, sizeof(global.errbuf), "Unknown error (Windows)");
EOD

    replace_function 'int tb_init_file' <<'EOD'
    if (global.initialized) return TB_ERR_INIT_ALREADY;
    if (strcmp(path, "/dev/tty")) return TB_ERR_WIN_UNSUPPORTED;
    return tb_init_fd(-1);
EOD

    my_sed 's/^(\s+)global\.(rfd|wfd|ttyfd) = -1;$/\1global.\2 = NULL;/'
    alter_function 'int tb_init_rwfd' <<'EOD'
    if (rfd != -1 || wfd != -1) return TB_ERR_WIN_UNSUPPORTED;
    global.rfd = GetStdHandle(STD_INPUT_HANDLE);
    global.wfd = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!global.rfd || global.rfd == INVALID_HANDLE_VALUE) return TB_ERR_WIN_NO_STDIO;
    if (!global.wfd || global.wfd == INVALID_HANDLE_VALUE) return TB_ERR_WIN_NO_STDIO;
    global.ttyfd = global.wfd;
    global.ttyfd_open = 1;
EOD

    alter_function 'static int tb_deinit' <<'EOD'
    if (global.has_orig_tios) {
        SetConsoleMode(global.rfd, global.orig_tios[0]);
        SetConsoleMode(global.wfd, global.orig_tios[1]);
    }
EOD

    my_sed 's/int bytebuf_flush\(struct bytebuf \*b, int fd\)/int bytebuf_flush(struct bytebuf *b, HANDLE fd)/'
    alter_function 'static int bytebuf_flush' <<'EOD'
    DWORD nw;
    if (!WriteConsoleA(fd, b->buf, (DWORD)b->len, &nw, NULL) || nw != b->len) {
        return TB_ERR; // TODO: GetLastError
    }
EOD

    replace_function 'static int init_term_attrs' <<'EOD'
    if (   !SetConsoleCP(CP_UTF8)
        || !SetConsoleOutputCP(CP_UTF8)) {
        return TB_ERR_WIN_SET_CONMODE;
    } else if (!GetConsoleMode(global.rfd, &global.orig_tios[0])
            || !GetConsoleMode(global.wfd, &global.orig_tios[1])
            || !(global.has_orig_tios = 1)) {
        return TB_ERR_WIN_GET_CONMODE;
    } else if (!SetConsoleMode(global.rfd, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_VIRTUAL_TERMINAL_INPUT)
            || !SetConsoleMode(global.wfd, ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN)) {
        return TB_ERR_WIN_SET_CONMODE;
    }
    return TB_OK;
EOD


    replace_function 'static int update_term_size' <<'EOD'
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(global.wfd, &info)) {
        return TB_ERR_WIN_RESIZE;
    }
    global.width = info.dwSize.X;
    global.height = info.dwSize.Y;
    return TB_OK;
EOD

    replace_function 'static int init_term_caps' <<'EOD'
    (void)load_terminfo;
    (void)parse_terminfo_caps;
    return load_builtin_caps();
EOD

    replace_function 'static int wait_event' <<'EOD'
    int rv;
    INPUT_RECORD buf[TB_OPT_READ_BUF];

    memset(event, 0, sizeof(*event));
    if_ok_return(rv, extract_event(event));

    do {
        DWORD wait_rv = WaitForSingleObject(global.rfd, (DWORD)timeout);
        if (wait_rv == WAIT_TIMEOUT) {
            return TB_ERR_NO_EVENT;
        } else if (wait_rv != WAIT_OBJECT_0) {
            return TB_ERR_POLL;
        }

        DWORD nevent = 0;
        if (!ReadConsoleInputA(global.rfd, buf, TB_OPT_READ_BUF, &nevent)) {
            return TB_ERR_READ;
        }

        DWORD i;
        for (i = 0; i < nevent; i++) {
            if (buf[i].EventType == KEY_EVENT) {
                if (buf[i].Event.KeyEvent.bKeyDown &&
                    buf[i].Event.KeyEvent.uChar.AsciiChar != 0)
                {
                    bytebuf_nputs(&global.in,
                        &(buf[i].Event.KeyEvent.uChar.AsciiChar), 1);
                }
            }
        }

        for (i = 0; i < nevent; i++) {
            if (buf[i].EventType == WINDOW_BUFFER_SIZE_EVENT) {
                if_err_return(rv, update_term_size());
                if_err_return(rv, resize_cellbufs());
                event->type = TB_EVENT_RESIZE;
                event->w = global.width;
                event->h = global.height;
                return TB_OK;
            }
        }

        memset(event, 0, sizeof(*event));
        if_ok_return(rv, extract_event(event));
    } while (timeout == -1);

    return rv;
EOD
}

alter_function() {
    local fsig=$1
    local fcode=$(cat)
    awk -va=0 -vb=0 -vfcode="$fcode" \
        "/^${fsig}\(.* {$/ { a=1; print; next }
        a==1 && /__posix_start/ { b=1 }
        b==0 { print }
        b==1 && /__posix_end/ { print fcode; b=0; a=0 }" \
        "$out" >"$tmp"
    mv -f "$tmp" "$out"
    echo "alter_function $fsig"
}

replace_function() {
    local fsig=$1
    local fbody=$(cat)
    awk -va=0 -vfbody="$fbody" \
        "/^${fsig}\(.* {$/ { a=1; print; next }
        a==0 { print }
        a==1 && /^}$/ { print fbody; print; a=0 }" \
        "$out" >"$tmp"
    mv -f "$tmp" "$out"
    echo "replace_function $fsig"
}

my_sed() {
    sed -i -E "$@" "$out"
    echo "sed ${1:-}"
}

main "$@"
