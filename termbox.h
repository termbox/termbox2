/*
MIT License

Copyright (c) 2010-2020 nsf <no.smile.face@gmail.com>
              2015-2021 Adam Saponara <as@php.net>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef __TERMBOX_H
#define __TERMBOX_H

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#if !TB_OPT_SELECT
#include <poll.h>
#else
#include <sys/select.h>
#endif
#include <limits.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <wchar.h>

#ifdef __cplusplus
extern "C" { // __ffi_strip
#endif

/* ASCII key constants (tb_event.key) */
#define TB_KEY_CTRL_TILDE       0x00
#define TB_KEY_CTRL_2           0x00 /* clash with 'CTRL_TILDE'     */
#define TB_KEY_CTRL_A           0x01
#define TB_KEY_CTRL_B           0x02
#define TB_KEY_CTRL_C           0x03
#define TB_KEY_CTRL_D           0x04
#define TB_KEY_CTRL_E           0x05
#define TB_KEY_CTRL_F           0x06
#define TB_KEY_CTRL_G           0x07
#define TB_KEY_BACKSPACE        0x08
#define TB_KEY_CTRL_H           0x08 /* clash with 'CTRL_BACKSPACE' */
#define TB_KEY_TAB              0x09
#define TB_KEY_CTRL_I           0x09 /* clash with 'TAB'            */
#define TB_KEY_CTRL_J           0x0a
#define TB_KEY_CTRL_K           0x0b
#define TB_KEY_CTRL_L           0x0c
#define TB_KEY_ENTER            0x0d
#define TB_KEY_CTRL_M           0x0d /* clash with 'ENTER'          */
#define TB_KEY_CTRL_N           0x0e
#define TB_KEY_CTRL_O           0x0f
#define TB_KEY_CTRL_P           0x10
#define TB_KEY_CTRL_Q           0x11
#define TB_KEY_CTRL_R           0x12
#define TB_KEY_CTRL_S           0x13
#define TB_KEY_CTRL_T           0x14
#define TB_KEY_CTRL_U           0x15
#define TB_KEY_CTRL_V           0x16
#define TB_KEY_CTRL_W           0x17
#define TB_KEY_CTRL_X           0x18
#define TB_KEY_CTRL_Y           0x19
#define TB_KEY_CTRL_Z           0x1a
#define TB_KEY_ESC              0x1b
#define TB_KEY_CTRL_LSQ_BRACKET 0x1b /* clash with 'ESC'            */
#define TB_KEY_CTRL_3           0x1b /* clash with 'ESC'            */
#define TB_KEY_CTRL_4           0x1c
#define TB_KEY_CTRL_BACKSLASH   0x1c /* clash with 'CTRL_4'         */
#define TB_KEY_CTRL_5           0x1d
#define TB_KEY_CTRL_RSQ_BRACKET 0x1d /* clash with 'CTRL_5'         */
#define TB_KEY_CTRL_6           0x1e
#define TB_KEY_CTRL_7           0x1f
#define TB_KEY_CTRL_SLASH       0x1f /* clash with 'CTRL_7'         */
#define TB_KEY_CTRL_UNDERSCORE  0x1f /* clash with 'CTRL_7'         */
#define TB_KEY_SPACE            0x20
#define TB_KEY_BACKSPACE2       0x7f
#define TB_KEY_CTRL_8           0x7f /* clash with 'BACKSPACE2'     */

#define tb_key_i(i)             0xffff - (i)
/* Terminal-dependent key constants (tb_event.key) and terminfo capabilities */
/* BEGIN codegen h */
/* Produced by ./codegen.sh on Sun, 19 Sep 2021 01:02:02 +0000 */
#define TB_KEY_F1               (0xffff - 0)
#define TB_KEY_F2               (0xffff - 1)
#define TB_KEY_F3               (0xffff - 2)
#define TB_KEY_F4               (0xffff - 3)
#define TB_KEY_F5               (0xffff - 4)
#define TB_KEY_F6               (0xffff - 5)
#define TB_KEY_F7               (0xffff - 6)
#define TB_KEY_F8               (0xffff - 7)
#define TB_KEY_F9               (0xffff - 8)
#define TB_KEY_F10              (0xffff - 9)
#define TB_KEY_F11              (0xffff - 10)
#define TB_KEY_F12              (0xffff - 11)
#define TB_KEY_INSERT           (0xffff - 12)
#define TB_KEY_DELETE           (0xffff - 13)
#define TB_KEY_HOME             (0xffff - 14)
#define TB_KEY_END              (0xffff - 15)
#define TB_KEY_PGUP             (0xffff - 16)
#define TB_KEY_PGDN             (0xffff - 17)
#define TB_KEY_ARROW_UP         (0xffff - 18)
#define TB_KEY_ARROW_DOWN       (0xffff - 19)
#define TB_KEY_ARROW_LEFT       (0xffff - 20)
#define TB_KEY_ARROW_RIGHT      (0xffff - 21)
#define TB_KEY_BACK_TAB         (0xffff - 22)
#define TB_KEY_MOUSE_LEFT       (0xffff - 23)
#define TB_KEY_MOUSE_RIGHT      (0xffff - 24)
#define TB_KEY_MOUSE_MIDDLE     (0xffff - 25)
#define TB_KEY_MOUSE_RELEASE    (0xffff - 26)
#define TB_KEY_MOUSE_WHEEL_UP   (0xffff - 27)
#define TB_KEY_MOUSE_WHEEL_DOWN (0xffff - 28)

#define TB_CAP_F1               0
#define TB_CAP_F2               1
#define TB_CAP_F3               2
#define TB_CAP_F4               3
#define TB_CAP_F5               4
#define TB_CAP_F6               5
#define TB_CAP_F7               6
#define TB_CAP_F8               7
#define TB_CAP_F9               8
#define TB_CAP_F10              9
#define TB_CAP_F11              10
#define TB_CAP_F12              11
#define TB_CAP_INSERT           12
#define TB_CAP_DELETE           13
#define TB_CAP_HOME             14
#define TB_CAP_END              15
#define TB_CAP_PGUP             16
#define TB_CAP_PGDN             17
#define TB_CAP_ARROW_UP         18
#define TB_CAP_ARROW_DOWN       19
#define TB_CAP_ARROW_LEFT       20
#define TB_CAP_ARROW_RIGHT      21
#define TB_CAP_BACK_TAB         22
#define TB_CAP__COUNT_KEYS      23
#define TB_CAP_ENTER_CA         23
#define TB_CAP_EXIT_CA          24
#define TB_CAP_SHOW_CURSOR      25
#define TB_CAP_HIDE_CURSOR      26
#define TB_CAP_CLEAR_SCREEN     27
#define TB_CAP_SGR0             28
#define TB_CAP_UNDERLINE        29
#define TB_CAP_BOLD             30
#define TB_CAP_BLINK            31
#define TB_CAP_ITALIC           32
#define TB_CAP_REVERSE          33
#define TB_CAP_ENTER_KEYPAD     34
#define TB_CAP_EXIT_KEYPAD      35
#define TB_CAP__COUNT           36
/* END codegen h */

/* Some hard-coded caps */
#define TB_HARDCAP_ENTER_MOUSE  "\x1b[?1000h\x1b[?1002h\x1b[?1015h\x1b[?1006h"
#define TB_HARDCAP_EXIT_MOUSE   "\x1b[?1006l\x1b[?1015l\x1b[?1002l\x1b[?1000l"

/* Colors (numeric) and attributes (bitwise) (tb_cell.fg, tb_cell.bg) */
#define TB_DEFAULT              0x0000
#define TB_BLACK                0x0001
#define TB_RED                  0x0002
#define TB_GREEN                0x0003
#define TB_YELLOW               0x0004
#define TB_BLUE                 0x0005
#define TB_MAGENTA              0x0006
#define TB_CYAN                 0x0007
#define TB_WHITE                0x0008
#define TB_BOLD                 0x0100
#define TB_UNDERLINE            0x0200
#define TB_REVERSE              0x0400
#define TB_ITALIC               0x0800

/* Event types (tb_event.type) */
#define TB_EVENT_KEY            1
#define TB_EVENT_RESIZE         2
#define TB_EVENT_MOUSE          3

/* Key modifiers (bitwise) (tb_event.mod) */
#define TB_MOD_ALT              1
#define TB_MOD_CTRL             2
#define TB_MOD_SHIFT            4
#define TB_MOD_MOTION           8

/* Input modes (bitwise) (tb_set_input_mode) */
#define TB_INPUT_CURRENT        0
#define TB_INPUT_ESC            1
#define TB_INPUT_ALT            2
#define TB_INPUT_MOUSE          4

/* Output modes (tb_set_output_mode) */
#define TB_OUTPUT_CURRENT       0
#define TB_OUTPUT_NORMAL        1
#define TB_OUTPUT_256           2
#define TB_OUTPUT_216           3
#define TB_OUTPUT_GRAYSCALE     4
#ifdef TB_OPT_TRUECOLOR
#define TB_OUTPUT_TRUECOLOR 5
#endif

/* Common function return values unless otherwise noted */
#define TB_OK                   0
#define TB_ERR                  -1
#define TB_ERR_NEED_MORE        -2
#define TB_ERR_INIT_ALREADY     -3
#define TB_ERR_INIT_OPEN        -4
#define TB_ERR_MEM              -5
#define TB_ERR_NO_EVENT         -6
#define TB_ERR_NO_TERM          -7
#define TB_ERR_NOT_INIT         -8
#define TB_ERR_OUT_OF_BOUNDS    -9
#define TB_ERR_READ             -10
#define TB_ERR_RESIZE_IOCTL     -11
#define TB_ERR_RESIZE_PIPE      -12
#define TB_ERR_RESIZE_SIGACTION -13
#define TB_ERR_POLL             -14
#define TB_ERR_TCGETATTR        -15
#define TB_ERR_TCSETATTR        -16
#define TB_ERR_UNSUPPORTED_TERM -17
#define TB_ERR_RESIZE_WRITE     -18
#define TB_ERR_RESIZE_POLL      -19
#define TB_ERR_RESIZE_READ      -20
#define TB_ERR_RESIZE_SSCANF    -21
#define TB_ERR_CAP_COLLISION    -22

#define TB_ERR_SELECT           TB_ERR_POLL
#define TB_ERR_RESIZE_SELECT    TB_ERR_RESIZE_POLL

/* Function types to be used with tb_set_func() */
#define TB_FUNC_EXTRACT_PRE     0
#define TB_FUNC_EXTRACT_POST    1

/* Define this to set the size of the buffer used in tb_printf()
 * and tb_sendf()
 */
#ifndef TB_OPT_PRINTF_BUF
#define TB_OPT_PRINTF_BUF 4096
#endif

/* Define this to set the size of the read buffer used when reading
 * from the tty
 */
#ifndef TB_OPT_READ_BUF
#define TB_OPT_READ_BUF 64
#endif

/* Define this for limited back compat with termbox v1 */
#ifdef TB_OPT_V1_COMPAT
#define tb_change_cell          tb_set_cell
#define tb_put_cell(x, y, c)    tb_set_cell((x), (y), (c)->ch, (c)->fg, (c)->bg)
#define tb_set_clear_attributes tb_set_clear_attrs
#define tb_select_input_mode    tb_set_input_mode
#define tb_select_output_mode   tb_set_output_mode
#endif

/* Define these to swap in a different allocator */
#ifndef tb_malloc
#define tb_malloc  malloc
#define tb_realloc realloc
#define tb_free    free
#endif

#ifdef TB_OPT_TRUECOLOR
typedef uint32_t uintattr_t;
#else
typedef uint16_t uintattr_t; // __ffi_strip
#endif

/* The terminal screen is represented as 2d array of cells. The structure is
 * optimized for dealing with single-width (wcwidth()==1) Unicode code points,
 * however some support for grapheme clusters (e.g., combining diacritical
 * marks) and wide code points (e.g., Hiragana) is provided through ech, nech,
 * cech via tb_set_cell_ex(). ech is only valid when nech>0, otherwise ch is
 * used.
 *
 * For non-single-width code points, given N=wcwidth(ch)/wcswidth(ech):
 *
 *   when N==0: termbox forces a single-width cell. Callers should avoid this
 *              if aiming to render text accurately.
 *
 *    when N>1: termbox zeroes out the following N-1 cells and skips sending
 *              them to the tty. So, e.g., if the caller sets x=0,y=0 to an N==2
 *              code point, the caller's next set should be at x=2,y=0. Anything
 *              set at x=1,y=0 will be ignored. If there are not enough columns
 *              remaining on the line to render N width, spaces are sent
 *              instead.
 *
 * See tb_present() for implementation.
 */
struct tb_cell {
    uint32_t ch;   /* a Unicode character */
    uintattr_t fg; /* bit-wise foreground attributes */
    uintattr_t bg; /* bit-wise background attributes */
#ifdef TB_OPT_EGC
    uint32_t *ech; /* a grapheme cluster of Unicode code points */
    size_t nech;   /* length in bytes of ech, 0 means use ch instead of ech */
    size_t cech;   /* capacity in bytes of ech */
#endif
};

/* An incoming event from the tty.
 *
 * Given the event type, the following fields are relevant:
 *
 *      when TB_EVENT_KEY: (key XOR ch, one will be zero), mod. Note there is
 *                         overlap between TB_MOD_CTRL and TB_KEY_CTRL_*.
 *                         TB_MOD_CTRL and TB_MOD_SHIFT are only set as
 *                         modifiers to TB_KEY_ARROW_*.
 *
 *   when TB_EVENT_RESIZE: w, h
 *
 *    when TB_EVENT_MOUSE: key (TB_KEY_MOUSE_*), x, y
 */
struct tb_event {
    uint8_t type; /* one of TB_EVENT_* constants */
    uint8_t mod;  /* bit-wise TB_MOD_* constants */
    uint16_t key; /* one of TB_KEY_* constants */
    uint32_t ch;  /* a Unicode code point */
    int32_t w;    /* resize width */
    int32_t h;    /* resize height */
    int32_t x;    /* mouse x */
    int32_t y;    /* mouse y */
};

/* Initializes the termbox library. This function should be called before any
 * other functions. tb_init() is equivalent to tb_init_file("/dev/tty"). After
 * successful initialization, the library must be finalized using the
 * tb_shutdown() function.
 */
int tb_init();
int tb_init_file(const char *path);
int tb_init_fd(int ttyfd);
int tb_init_rwfd(int rfd, int wfd);
int tb_shutdown();

/* Returns the size of the internal back buffer (which is the same as terminal's
 * window size in rows and columns). The internal buffer can be resized after
 * tb_clear() or tb_present() function calls. Both dimensions have an
 * unspecified negative value when called before tb_init() or after
 * tb_shutdown().
 */
int tb_width();
int tb_height();

/* Clears the internal back buffer using TB_DEFAULT color or the
 * color/attributes set by tb_set_clear_attrs() function.
 */
int tb_clear();
int tb_set_clear_attrs(uintattr_t fg, uintattr_t bg);

/* Synchronizes the internal back buffer with the terminal by writing to tty. */
int tb_present();

/* Sets the position of the cursor. Upper-left character is (0, 0). */
int tb_set_cursor(int cx, int cy);
int tb_hide_cursor();

/* Set cell contents in the internal back buffer at the specified position. Use
 * tb_set_cell_ex() for rendering grapheme clusters (e.g., combining diacritical
 * marks). Function tb_set_cell(x, y, ch, fg, bg) is equivalent to
 * tb_set_cell_ex(x, y, &ch, 1, fg, bg). tb_extend_cell() is a shortcut for
 * appending 1 code point to cell->ech.
 */
int tb_set_cell(int x, int y, uint32_t ch, uintattr_t fg, uintattr_t bg);
int tb_set_cell_ex(int x, int y, uint32_t *ch, size_t nch, uintattr_t fg,
    uintattr_t bg);
int tb_extend_cell(int x, int y, uint32_t ch);

/* Sets the input mode. Termbox has two input modes:
 * 1. TB_INPUT_ESC
 *    When escape (\x1b) is in the buffer and there's no match for an escape
 *    sequence, a key event for TB_KEY_ESC is returned.
 * 2. TB_INPUT_ALT
 *    When escape (\x1b) is in the buffer and there's no match for an escape
 *    sequence, the next keyboard event is returned with a TB_MOD_ALT modifier.
 *
 * You can also apply TB_INPUT_MOUSE via bitwise OR operation to either of the
 * modes (e.g., TB_INPUT_ESC | TB_INPUT_MOUSE) to receive TB_EVENT_MOUSE events.
 * If none of the main two modes were set, but the mouse mode was, TB_INPUT_ESC
 * mode is used. If for some reason you've decided to use
 * (TB_INPUT_ESC | TB_INPUT_ALT) combination, it will behave as if only
 * TB_INPUT_ESC was selected.
 *
 * If mode is TB_INPUT_CURRENT, the function returns the current input mode.
 *
 * The default input mode is TB_INPUT_ESC.
 */
int tb_set_input_mode(int mode);

/* Sets the termbox output mode. Termbox has three output modes:
 * 1. TB_OUTPUT_NORMAL     => [1..8]
 *    This mode provides 8 different colors:
 *      TB_BLACK, TB_RED, TB_GREEN, TB_YELLOW,
 *      TB_BLUE, TB_MAGENTA, TB_CYAN, TB_WHITE
 *    Colors may be bitwise OR'd with attributes:
 *      TB_BOLD, TB_UNDERLINE, TB_REVERSE, TB_ITALIC
 *
 *    Example usage:
 *      tb_set_cell(x, y, '@', TB_BLACK | TB_BOLD, TB_RED);
 *
 * 2. TB_OUTPUT_256        => [0..255]
 *    In this mode you get 256 distinct colors:
 *      0x00 - 0x07: the 8 colors as in TB_OUTPUT_NORMAL
 *      0x08 - 0x0f: bright versions of the above
 *      0x10 - 0xe7: 216 different colors
 *      0xe8 - 0xff: 24 different shades of grey
 *
 *    Example usage:
 *      tb_set_cell(x, y, '@', 184, 240);
 *      tb_set_cell(x, y, '@', 0xb8, 0xf0);
 *
 * 3. TB_OUTPUT_216        => [0..215]
 *    This mode supports the 3rd range of TB_OUTPUT_256 only, but you don't need
 *    to provide an offset.
 *
 * 4. TB_OUTPUT_GRAYSCALE  => [0..23]
 *    This mode supports the 4th range of TB_OUTPUT_256 only, but you dont need
 *    to provide an offset.
 *
 * 5. TB_OUTPUT_TRUECOLOR  => [0x000000..0xffffff]
 *    This mode provides 24-bit color on supported terminals. The format is
 *    0xRRGGBB.
 *
 * If mode is TB_OUTPUT_CURRENT, the function returns the current output mode.
 *
 * The default output mode is TB_OUTPUT_NORMAL.
 */
int tb_set_output_mode(int mode);

/* Wait for an event up to timeout_ms milliseconds and fill the event structure
 * with it. If no event is available within the timeout period, TB_ERR_NO_EVENT
 * is returned. On a resize event, the underlying poll(2) call may be
 * interrupted, yielding a return code of TB_ERR_POLL. In this case, you may
 * check errno via tb_last_errno(). If it's EINTR, you can safely ignore that
 * and call tb_peek_event() again.
 */
int tb_peek_event(struct tb_event *event, int timeout_ms);

/* Same as tb_peek_event except no timeout. */
int tb_poll_event(struct tb_event *event);

/* Internal termbox FDs that can be used with poll() / select(). Must call
 * tb_poll_event() / tb_peek_event() if activity is detected. */
int tb_get_fds(int *ttyfd, int *resizefd);

/* Print and printf functions. Specify param out_w to determine width of printed
 * string.
 */
int tb_print(int x, int y, uintattr_t fg, uintattr_t bg, const char *str);
int tb_printf(int x, int y, uintattr_t fg, uintattr_t bg, const char *fmt, ...);
int tb_print_ex(int x, int y, uintattr_t fg, uintattr_t bg, size_t *out_w,
    const char *str);
int tb_printf_ex(int x, int y, uintattr_t fg, uintattr_t bg, size_t *out_w,
    const char *fmt, ...);

/* Send raw bytes to terminal. */
int tb_send(const char *buf, size_t nbuf);
int tb_sendf(const char *fmt, ...);

/* Set custom functions. fn_type is one of TB_FUNC_* constants, fn is a
 * compatible function pointer, or NULL to clear.
 *
 * TB_FUNC_EXTRACT_PRE:
 *   If specified, invoke this function BEFORE termbox tries to extract any
 *   escape sequences from the input buffer.
 *
 * TB_FUNC_EXTRACT_POST:
 *   If specified, invoke this function AFTER termbox tries (and fails) to
 *   extract any escape sequences from the input buffer.
 */
int tb_set_func(int fn_type, int (*fn)(struct tb_event *, size_t *));

/* Utility functions. */
int tb_utf8_char_length(char c);
int tb_utf8_char_to_unicode(uint32_t *out, const char *c);
int tb_utf8_unicode_to_char(char *out, uint32_t c);
int tb_last_errno();
const char *tb_strerror(int err);

struct tb_cell *tb_cell_buffer();

#ifdef __cplusplus
} // __ffi_strip
#endif

#endif /* __TERMBOX_H */

#ifdef TB_IMPL

#ifndef TB_OPT_TRUECOLOR
#define TB_OUTPUT_TRUECOLOR (-1)
#endif

#define if_err_return(rv, expr)                                                \
    if (((rv) = (expr)) != TB_OK)                                              \
    return (rv)
#define if_err_break(rv, expr)                                                 \
    if (((rv) = (expr)) != TB_OK)                                              \
    break
#define if_ok_return(rv, expr)                                                 \
    if (((rv) = (expr)) == TB_OK)                                              \
    return (rv)
#define if_ok_or_need_more_return(rv, expr)                                    \
    if (((rv) = (expr)) == TB_OK || (rv) == TB_ERR_NEED_MORE)                  \
    return (rv)

#define send_literal(rv, a)                                                    \
    if_err_return((rv), bytebuf_nputs(&global.out, (a), sizeof(a) - 1))

#define send_num(rv, nbuf, n)                                                  \
    if_err_return((rv),                                                        \
        bytebuf_nputs(&global.out, (nbuf), convert_num((n), (nbuf))))

#define snprintf_or_return(rv, str, sz, fmt, ...)                              \
    do {                                                                       \
        (rv) = snprintf((str), (sz), (fmt), __VA_ARGS__);                      \
        if ((rv) < 0 || (rv) >= (int)(sz))                                     \
            return TB_ERR;                                                     \
    } while (0)

#define if_not_init_return()                                                   \
    if (!global.initialized)                                                   \
    return TB_ERR_NOT_INIT

struct bytebuf_t {
    char *buf;
    size_t len;
    size_t cap;
};

struct cellbuf_t {
    int width;
    int height;
    struct tb_cell *cells;
};

struct cap_trie_t {
    char c;
    struct cap_trie_t *children;
    size_t nchildren;
    int is_leaf;
    uint16_t key;
    uint8_t mod;
};

struct tb_global_t {
    int ttyfd;
    int rfd;
    int wfd;
    int ttyfd_open;
    int resize_pipefd[2];
    int width;
    int height;
    int cursor_x;
    int cursor_y;
    int last_x;
    int last_y;
    uintattr_t fg;
    uintattr_t bg;
    uintattr_t last_fg;
    uintattr_t last_bg;
    int input_mode;
    int output_mode;
    char *terminfo;
    size_t nterminfo;
    const char *caps[TB_CAP__COUNT];
    struct cap_trie_t cap_trie;
    struct bytebuf_t in;
    struct bytebuf_t out;
    struct cellbuf_t back;
    struct cellbuf_t front;
    struct termios orig_tios;
    int has_orig_tios;
    int last_errno;
    int initialized;
    int need_resize;
    int (*fn_extract_esc_pre)(struct tb_event *, size_t *);
    int (*fn_extract_esc_post)(struct tb_event *, size_t *);
};

struct tb_global_t global = {0};

/* BEGIN codegen c */
/* Produced by ./codegen.sh on Sun, 19 Sep 2021 01:02:03 +0000 */

static const int16_t terminfo_cap_indexes[] = {
    66,  // kf1 (TB_CAP_F1)
    68,  // kf2 (TB_CAP_F2)
    69,  // kf3 (TB_CAP_F3)
    70,  // kf4 (TB_CAP_F4)
    71,  // kf5 (TB_CAP_F5)
    72,  // kf6 (TB_CAP_F6)
    73,  // kf7 (TB_CAP_F7)
    74,  // kf8 (TB_CAP_F8)
    75,  // kf9 (TB_CAP_F9)
    67,  // kf10 (TB_CAP_F10)
    216, // kf11 (TB_CAP_F11)
    217, // kf12 (TB_CAP_F12)
    77,  // kich1 (TB_CAP_INSERT)
    59,  // kdch1 (TB_CAP_DELETE)
    76,  // khome (TB_CAP_HOME)
    164, // kend (TB_CAP_END)
    82,  // kpp (TB_CAP_PGUP)
    81,  // knp (TB_CAP_PGDN)
    87,  // kcuu1 (TB_CAP_ARROW_UP)
    61,  // kcud1 (TB_CAP_ARROW_DOWN)
    79,  // kcub1 (TB_CAP_ARROW_LEFT)
    83,  // kcuf1 (TB_CAP_ARROW_RIGHT)
    148, // kcbt (TB_CAP_BACK_TAB)
    28,  // smcup (TB_CAP_ENTER_CA)
    40,  // rmcup (TB_CAP_EXIT_CA)
    16,  // cnorm (TB_CAP_SHOW_CURSOR)
    13,  // civis (TB_CAP_HIDE_CURSOR)
    5,   // clear (TB_CAP_CLEAR_SCREEN)
    39,  // sgr0 (TB_CAP_SGR0)
    36,  // smul (TB_CAP_UNDERLINE)
    27,  // bold (TB_CAP_BOLD)
    26,  // blink (TB_CAP_BLINK)
    311, // sitm (TB_CAP_ITALIC)
    34,  // rev (TB_CAP_REVERSE)
    89,  // smkx (TB_CAP_ENTER_KEYPAD)
    88,  // rmkx (TB_CAP_EXIT_KEYPAD)
};

// xterm
static const char *xterm_caps[] = {
    "\033OP",                  // kf1 (TB_CAP_F1)
    "\033OQ",                  // kf2 (TB_CAP_F2)
    "\033OR",                  // kf3 (TB_CAP_F3)
    "\033OS",                  // kf4 (TB_CAP_F4)
    "\033[15~",                // kf5 (TB_CAP_F5)
    "\033[17~",                // kf6 (TB_CAP_F6)
    "\033[18~",                // kf7 (TB_CAP_F7)
    "\033[19~",                // kf8 (TB_CAP_F8)
    "\033[20~",                // kf9 (TB_CAP_F9)
    "\033[21~",                // kf10 (TB_CAP_F10)
    "\033[23~",                // kf11 (TB_CAP_F11)
    "\033[24~",                // kf12 (TB_CAP_F12)
    "\033[2~",                 // kich1 (TB_CAP_INSERT)
    "\033[3~",                 // kdch1 (TB_CAP_DELETE)
    "\033OH",                  // khome (TB_CAP_HOME)
    "\033OF",                  // kend (TB_CAP_END)
    "\033[5~",                 // kpp (TB_CAP_PGUP)
    "\033[6~",                 // knp (TB_CAP_PGDN)
    "\033OA",                  // kcuu1 (TB_CAP_ARROW_UP)
    "\033OB",                  // kcud1 (TB_CAP_ARROW_DOWN)
    "\033OD",                  // kcub1 (TB_CAP_ARROW_LEFT)
    "\033OC",                  // kcuf1 (TB_CAP_ARROW_RIGHT)
    "\033[Z",                  // kcbt (TB_CAP_BACK_TAB)
    "\033[?1049h\033[22;0;0t", // smcup (TB_CAP_ENTER_CA)
    "\033[?1049l\033[23;0;0t", // rmcup (TB_CAP_EXIT_CA)
    "\033[?12l\033[?25h",      // cnorm (TB_CAP_SHOW_CURSOR)
    "\033[?25l",               // civis (TB_CAP_HIDE_CURSOR)
    "\033[H\033[2J",           // clear (TB_CAP_CLEAR_SCREEN)
    "\033(B\033[m",            // sgr0 (TB_CAP_SGR0)
    "\033[4m",                 // smul (TB_CAP_UNDERLINE)
    "\033[1m",                 // bold (TB_CAP_BOLD)
    "\033[5m",                 // blink (TB_CAP_BLINK)
    "\033[3m",                 // sitm (TB_CAP_ITALIC)
    "\033[7m",                 // rev (TB_CAP_REVERSE)
    "\033[?1h\033=",           // smkx (TB_CAP_ENTER_KEYPAD)
    "\033[?1l\033>",           // rmkx (TB_CAP_EXIT_KEYPAD)
};

// linux
static const char *linux_caps[] = {
    "\033[[A",           // kf1 (TB_CAP_F1)
    "\033[[B",           // kf2 (TB_CAP_F2)
    "\033[[C",           // kf3 (TB_CAP_F3)
    "\033[[D",           // kf4 (TB_CAP_F4)
    "\033[[E",           // kf5 (TB_CAP_F5)
    "\033[17~",          // kf6 (TB_CAP_F6)
    "\033[18~",          // kf7 (TB_CAP_F7)
    "\033[19~",          // kf8 (TB_CAP_F8)
    "\033[20~",          // kf9 (TB_CAP_F9)
    "\033[21~",          // kf10 (TB_CAP_F10)
    "\033[23~",          // kf11 (TB_CAP_F11)
    "\033[24~",          // kf12 (TB_CAP_F12)
    "\033[2~",           // kich1 (TB_CAP_INSERT)
    "\033[3~",           // kdch1 (TB_CAP_DELETE)
    "\033[1~",           // khome (TB_CAP_HOME)
    "\033[4~",           // kend (TB_CAP_END)
    "\033[5~",           // kpp (TB_CAP_PGUP)
    "\033[6~",           // knp (TB_CAP_PGDN)
    "\033[A",            // kcuu1 (TB_CAP_ARROW_UP)
    "\033[B",            // kcud1 (TB_CAP_ARROW_DOWN)
    "\033[D",            // kcub1 (TB_CAP_ARROW_LEFT)
    "\033[C",            // kcuf1 (TB_CAP_ARROW_RIGHT)
    "\033[Z",            // kcbt (TB_CAP_BACK_TAB)
    "",                  // smcup (TB_CAP_ENTER_CA)
    "",                  // rmcup (TB_CAP_EXIT_CA)
    "\033[?25h\033[?0c", // cnorm (TB_CAP_SHOW_CURSOR)
    "\033[?25l\033[?1c", // civis (TB_CAP_HIDE_CURSOR)
    "\033[H\033[J",      // clear (TB_CAP_CLEAR_SCREEN)
    "\033[m\017",        // sgr0 (TB_CAP_SGR0)
    "\033[4m",           // smul (TB_CAP_UNDERLINE)
    "\033[1m",           // bold (TB_CAP_BOLD)
    "\033[5m",           // blink (TB_CAP_BLINK)
    "",                  // sitm (TB_CAP_ITALIC)
    "\033[7m",           // rev (TB_CAP_REVERSE)
    "",                  // smkx (TB_CAP_ENTER_KEYPAD)
    "",                  // rmkx (TB_CAP_EXIT_KEYPAD)
};

// screen
static const char *screen_caps[] = {
    "\033OP",            // kf1 (TB_CAP_F1)
    "\033OQ",            // kf2 (TB_CAP_F2)
    "\033OR",            // kf3 (TB_CAP_F3)
    "\033OS",            // kf4 (TB_CAP_F4)
    "\033[15~",          // kf5 (TB_CAP_F5)
    "\033[17~",          // kf6 (TB_CAP_F6)
    "\033[18~",          // kf7 (TB_CAP_F7)
    "\033[19~",          // kf8 (TB_CAP_F8)
    "\033[20~",          // kf9 (TB_CAP_F9)
    "\033[21~",          // kf10 (TB_CAP_F10)
    "\033[23~",          // kf11 (TB_CAP_F11)
    "\033[24~",          // kf12 (TB_CAP_F12)
    "\033[2~",           // kich1 (TB_CAP_INSERT)
    "\033[3~",           // kdch1 (TB_CAP_DELETE)
    "\033[1~",           // khome (TB_CAP_HOME)
    "\033[4~",           // kend (TB_CAP_END)
    "\033[5~",           // kpp (TB_CAP_PGUP)
    "\033[6~",           // knp (TB_CAP_PGDN)
    "\033OA",            // kcuu1 (TB_CAP_ARROW_UP)
    "\033OB",            // kcud1 (TB_CAP_ARROW_DOWN)
    "\033OD",            // kcub1 (TB_CAP_ARROW_LEFT)
    "\033OC",            // kcuf1 (TB_CAP_ARROW_RIGHT)
    "\033[Z",            // kcbt (TB_CAP_BACK_TAB)
    "\033[?1049h",       // smcup (TB_CAP_ENTER_CA)
    "\033[?1049l",       // rmcup (TB_CAP_EXIT_CA)
    "\033[34h\033[?25h", // cnorm (TB_CAP_SHOW_CURSOR)
    "\033[?25l",         // civis (TB_CAP_HIDE_CURSOR)
    "\033[H\033[J",      // clear (TB_CAP_CLEAR_SCREEN)
    "\033[m\017",        // sgr0 (TB_CAP_SGR0)
    "\033[4m",           // smul (TB_CAP_UNDERLINE)
    "\033[1m",           // bold (TB_CAP_BOLD)
    "\033[5m",           // blink (TB_CAP_BLINK)
    "",                  // sitm (TB_CAP_ITALIC)
    "\033[7m",           // rev (TB_CAP_REVERSE)
    "\033[?1h\033=",     // smkx (TB_CAP_ENTER_KEYPAD)
    "\033[?1l\033>",     // rmkx (TB_CAP_EXIT_KEYPAD)
};

// rxvt-256color
static const char *rxvt_256color_caps[] = {
    "\033[11~",              // kf1 (TB_CAP_F1)
    "\033[12~",              // kf2 (TB_CAP_F2)
    "\033[13~",              // kf3 (TB_CAP_F3)
    "\033[14~",              // kf4 (TB_CAP_F4)
    "\033[15~",              // kf5 (TB_CAP_F5)
    "\033[17~",              // kf6 (TB_CAP_F6)
    "\033[18~",              // kf7 (TB_CAP_F7)
    "\033[19~",              // kf8 (TB_CAP_F8)
    "\033[20~",              // kf9 (TB_CAP_F9)
    "\033[21~",              // kf10 (TB_CAP_F10)
    "\033[23~",              // kf11 (TB_CAP_F11)
    "\033[24~",              // kf12 (TB_CAP_F12)
    "\033[2~",               // kich1 (TB_CAP_INSERT)
    "\033[3~",               // kdch1 (TB_CAP_DELETE)
    "\033[7~",               // khome (TB_CAP_HOME)
    "\033[8~",               // kend (TB_CAP_END)
    "\033[5~",               // kpp (TB_CAP_PGUP)
    "\033[6~",               // knp (TB_CAP_PGDN)
    "\033[A",                // kcuu1 (TB_CAP_ARROW_UP)
    "\033[B",                // kcud1 (TB_CAP_ARROW_DOWN)
    "\033[D",                // kcub1 (TB_CAP_ARROW_LEFT)
    "\033[C",                // kcuf1 (TB_CAP_ARROW_RIGHT)
    "\033[Z",                // kcbt (TB_CAP_BACK_TAB)
    "\0337\033[?47h",        // smcup (TB_CAP_ENTER_CA)
    "\033[2J\033[?47l\0338", // rmcup (TB_CAP_EXIT_CA)
    "\033[?25h",             // cnorm (TB_CAP_SHOW_CURSOR)
    "\033[?25l",             // civis (TB_CAP_HIDE_CURSOR)
    "\033[H\033[2J",         // clear (TB_CAP_CLEAR_SCREEN)
    "\033[m\017",            // sgr0 (TB_CAP_SGR0)
    "\033[4m",               // smul (TB_CAP_UNDERLINE)
    "\033[1m",               // bold (TB_CAP_BOLD)
    "\033[5m",               // blink (TB_CAP_BLINK)
    "",                      // sitm (TB_CAP_ITALIC)
    "\033[7m",               // rev (TB_CAP_REVERSE)
    "\033=",                 // smkx (TB_CAP_ENTER_KEYPAD)
    "\033>",                 // rmkx (TB_CAP_EXIT_KEYPAD)
};

// rxvt-unicode
static const char *rxvt_unicode_caps[] = {
    "\033[11~",           // kf1 (TB_CAP_F1)
    "\033[12~",           // kf2 (TB_CAP_F2)
    "\033[13~",           // kf3 (TB_CAP_F3)
    "\033[14~",           // kf4 (TB_CAP_F4)
    "\033[15~",           // kf5 (TB_CAP_F5)
    "\033[17~",           // kf6 (TB_CAP_F6)
    "\033[18~",           // kf7 (TB_CAP_F7)
    "\033[19~",           // kf8 (TB_CAP_F8)
    "\033[20~",           // kf9 (TB_CAP_F9)
    "\033[21~",           // kf10 (TB_CAP_F10)
    "\033[23~",           // kf11 (TB_CAP_F11)
    "\033[24~",           // kf12 (TB_CAP_F12)
    "\033[2~",            // kich1 (TB_CAP_INSERT)
    "\033[3~",            // kdch1 (TB_CAP_DELETE)
    "\033[7~",            // khome (TB_CAP_HOME)
    "\033[8~",            // kend (TB_CAP_END)
    "\033[5~",            // kpp (TB_CAP_PGUP)
    "\033[6~",            // knp (TB_CAP_PGDN)
    "\033[A",             // kcuu1 (TB_CAP_ARROW_UP)
    "\033[B",             // kcud1 (TB_CAP_ARROW_DOWN)
    "\033[D",             // kcub1 (TB_CAP_ARROW_LEFT)
    "\033[C",             // kcuf1 (TB_CAP_ARROW_RIGHT)
    "\033[Z",             // kcbt (TB_CAP_BACK_TAB)
    "\033[?1049h",        // smcup (TB_CAP_ENTER_CA)
    "\033[r\033[?1049l",  // rmcup (TB_CAP_EXIT_CA)
    "\033[?12l\033[?25h", // cnorm (TB_CAP_SHOW_CURSOR)
    "\033[?25l",          // civis (TB_CAP_HIDE_CURSOR)
    "\033[H\033[2J",      // clear (TB_CAP_CLEAR_SCREEN)
    "\033[m\033(B",       // sgr0 (TB_CAP_SGR0)
    "\033[4m",            // smul (TB_CAP_UNDERLINE)
    "\033[1m",            // bold (TB_CAP_BOLD)
    "\033[5m",            // blink (TB_CAP_BLINK)
    "\033[3m",            // sitm (TB_CAP_ITALIC)
    "\033[7m",            // rev (TB_CAP_REVERSE)
    "\033=",              // smkx (TB_CAP_ENTER_KEYPAD)
    "\033>",              // rmkx (TB_CAP_EXIT_KEYPAD)
};

// Eterm
static const char *eterm_caps[] = {
    "\033[11~",              // kf1 (TB_CAP_F1)
    "\033[12~",              // kf2 (TB_CAP_F2)
    "\033[13~",              // kf3 (TB_CAP_F3)
    "\033[14~",              // kf4 (TB_CAP_F4)
    "\033[15~",              // kf5 (TB_CAP_F5)
    "\033[17~",              // kf6 (TB_CAP_F6)
    "\033[18~",              // kf7 (TB_CAP_F7)
    "\033[19~",              // kf8 (TB_CAP_F8)
    "\033[20~",              // kf9 (TB_CAP_F9)
    "\033[21~",              // kf10 (TB_CAP_F10)
    "\033[23~",              // kf11 (TB_CAP_F11)
    "\033[24~",              // kf12 (TB_CAP_F12)
    "\033[2~",               // kich1 (TB_CAP_INSERT)
    "\033[3~",               // kdch1 (TB_CAP_DELETE)
    "\033[7~",               // khome (TB_CAP_HOME)
    "\033[8~",               // kend (TB_CAP_END)
    "\033[5~",               // kpp (TB_CAP_PGUP)
    "\033[6~",               // knp (TB_CAP_PGDN)
    "\033[A",                // kcuu1 (TB_CAP_ARROW_UP)
    "\033[B",                // kcud1 (TB_CAP_ARROW_DOWN)
    "\033[D",                // kcub1 (TB_CAP_ARROW_LEFT)
    "\033[C",                // kcuf1 (TB_CAP_ARROW_RIGHT)
    "",                      // kcbt (TB_CAP_BACK_TAB)
    "\0337\033[?47h",        // smcup (TB_CAP_ENTER_CA)
    "\033[2J\033[?47l\0338", // rmcup (TB_CAP_EXIT_CA)
    "\033[?25h",             // cnorm (TB_CAP_SHOW_CURSOR)
    "\033[?25l",             // civis (TB_CAP_HIDE_CURSOR)
    "\033[H\033[2J",         // clear (TB_CAP_CLEAR_SCREEN)
    "\033[m\017",            // sgr0 (TB_CAP_SGR0)
    "\033[4m",               // smul (TB_CAP_UNDERLINE)
    "\033[1m",               // bold (TB_CAP_BOLD)
    "\033[5m",               // blink (TB_CAP_BLINK)
    "",                      // sitm (TB_CAP_ITALIC)
    "\033[7m",               // rev (TB_CAP_REVERSE)
    "",                      // smkx (TB_CAP_ENTER_KEYPAD)
    "",                      // rmkx (TB_CAP_EXIT_KEYPAD)
};

static struct {
    const char *name;
    const char **caps;
    const char *alias;
} builtin_terms[] = {
    {"xterm",         xterm_caps,         ""    },
    {"linux",         linux_caps,         ""    },
    {"screen",        screen_caps,        "tmux"},
    {"rxvt-256color", rxvt_256color_caps, ""    },
    {"rxvt-unicode",  rxvt_unicode_caps,  "rxvt"},
    {"Eterm",         eterm_caps,         ""    },
    {NULL,            NULL,               NULL  },
};

/* END codegen c */

static struct {
    const char *cap;
    const uint16_t key;
    const uint8_t mod;
} builtin_mod_caps[] = {
  // xterm arrows
    {"\x1b[1;2A",    TB_KEY_ARROW_UP,    TB_MOD_SHIFT                           },
    {"\x1b[1;3A",    TB_KEY_ARROW_UP,    TB_MOD_ALT                             },
    {"\x1b[1;4A",    TB_KEY_ARROW_UP,    TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[1;5A",    TB_KEY_ARROW_UP,    TB_MOD_CTRL                            },
    {"\x1b[1;6A",    TB_KEY_ARROW_UP,    TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[1;7A",    TB_KEY_ARROW_UP,    TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b[1;8A",    TB_KEY_ARROW_UP,    TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},

    {"\x1b[1;2B",    TB_KEY_ARROW_DOWN,  TB_MOD_SHIFT                           },
    {"\x1b[1;3B",    TB_KEY_ARROW_DOWN,  TB_MOD_ALT                             },
    {"\x1b[1;4B",    TB_KEY_ARROW_DOWN,  TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[1;5B",    TB_KEY_ARROW_DOWN,  TB_MOD_CTRL                            },
    {"\x1b[1;6B",    TB_KEY_ARROW_DOWN,  TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[1;7B",    TB_KEY_ARROW_DOWN,  TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b[1;8B",    TB_KEY_ARROW_DOWN,  TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},

    {"\x1b[1;2C",    TB_KEY_ARROW_RIGHT, TB_MOD_SHIFT                           },
    {"\x1b[1;3C",    TB_KEY_ARROW_RIGHT, TB_MOD_ALT                             },
    {"\x1b[1;4C",    TB_KEY_ARROW_RIGHT, TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[1;5C",    TB_KEY_ARROW_RIGHT, TB_MOD_CTRL                            },
    {"\x1b[1;6C",    TB_KEY_ARROW_RIGHT, TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[1;7C",    TB_KEY_ARROW_RIGHT, TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b[1;8C",    TB_KEY_ARROW_RIGHT, TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},

    {"\x1b[1;2D",    TB_KEY_ARROW_LEFT,  TB_MOD_SHIFT                           },
    {"\x1b[1;3D",    TB_KEY_ARROW_LEFT,  TB_MOD_ALT                             },
    {"\x1b[1;4D",    TB_KEY_ARROW_LEFT,  TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[1;5D",    TB_KEY_ARROW_LEFT,  TB_MOD_CTRL                            },
    {"\x1b[1;6D",    TB_KEY_ARROW_LEFT,  TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[1;7D",    TB_KEY_ARROW_LEFT,  TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b[1;8D",    TB_KEY_ARROW_LEFT,  TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},

 // xterm keys
    {"\x1b[1;2H",    TB_KEY_HOME,        TB_MOD_SHIFT                           },
    {"\x1b[1;3H",    TB_KEY_HOME,        TB_MOD_ALT                             },
    {"\x1b[1;4H",    TB_KEY_HOME,        TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[1;5H",    TB_KEY_HOME,        TB_MOD_CTRL                            },
    {"\x1b[1;6H",    TB_KEY_HOME,        TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[1;7H",    TB_KEY_HOME,        TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b[1;8H",    TB_KEY_HOME,        TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},

    {"\x1b[1;2F",    TB_KEY_END,         TB_MOD_SHIFT                           },
    {"\x1b[1;3F",    TB_KEY_END,         TB_MOD_ALT                             },
    {"\x1b[1;4F",    TB_KEY_END,         TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[1;5F",    TB_KEY_END,         TB_MOD_CTRL                            },
    {"\x1b[1;6F",    TB_KEY_END,         TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[1;7F",    TB_KEY_END,         TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b[1;8F",    TB_KEY_END,         TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},

    {"\x1b[2;2~",    TB_KEY_INSERT,      TB_MOD_SHIFT                           },
    {"\x1b[2;3~",    TB_KEY_INSERT,      TB_MOD_ALT                             },
    {"\x1b[2;4~",    TB_KEY_INSERT,      TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[2;5~",    TB_KEY_INSERT,      TB_MOD_CTRL                            },
    {"\x1b[2;6~",    TB_KEY_INSERT,      TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[2;7~",    TB_KEY_INSERT,      TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b[2;8~",    TB_KEY_INSERT,      TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},

    {"\x1b[3;2~",    TB_KEY_DELETE,      TB_MOD_SHIFT                           },
    {"\x1b[3;3~",    TB_KEY_DELETE,      TB_MOD_ALT                             },
    {"\x1b[3;4~",    TB_KEY_DELETE,      TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[3;5~",    TB_KEY_DELETE,      TB_MOD_CTRL                            },
    {"\x1b[3;6~",    TB_KEY_DELETE,      TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[3;7~",    TB_KEY_DELETE,      TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b[3;8~",    TB_KEY_DELETE,      TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},

    {"\x1b[5;2~",    TB_KEY_PGUP,        TB_MOD_SHIFT                           },
    {"\x1b[5;3~",    TB_KEY_PGUP,        TB_MOD_ALT                             },
    {"\x1b[5;4~",    TB_KEY_PGUP,        TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[5;5~",    TB_KEY_PGUP,        TB_MOD_CTRL                            },
    {"\x1b[5;6~",    TB_KEY_PGUP,        TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[5;7~",    TB_KEY_PGUP,        TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b[5;8~",    TB_KEY_PGUP,        TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},

    {"\x1b[6;2~",    TB_KEY_PGDN,        TB_MOD_SHIFT                           },
    {"\x1b[6;3~",    TB_KEY_PGDN,        TB_MOD_ALT                             },
    {"\x1b[6;4~",    TB_KEY_PGDN,        TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[6;5~",    TB_KEY_PGDN,        TB_MOD_CTRL                            },
    {"\x1b[6;6~",    TB_KEY_PGDN,        TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[6;7~",    TB_KEY_PGDN,        TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b[6;8~",    TB_KEY_PGDN,        TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},

    {"\x1b[1;2P",    TB_KEY_F1,          TB_MOD_SHIFT                           },
    {"\x1b[1;3P",    TB_KEY_F1,          TB_MOD_ALT                             },
    {"\x1b[1;4P",    TB_KEY_F1,          TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[1;5P",    TB_KEY_F1,          TB_MOD_CTRL                            },
    {"\x1b[1;6P",    TB_KEY_F1,          TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[1;7P",    TB_KEY_F1,          TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b[1;8P",    TB_KEY_F1,          TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},

    {"\x1b[1;2Q",    TB_KEY_F2,          TB_MOD_SHIFT                           },
    {"\x1b[1;3Q",    TB_KEY_F2,          TB_MOD_ALT                             },
    {"\x1b[1;4Q",    TB_KEY_F2,          TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[1;5Q",    TB_KEY_F2,          TB_MOD_CTRL                            },
    {"\x1b[1;6Q",    TB_KEY_F2,          TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[1;7Q",    TB_KEY_F2,          TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b[1;8Q",    TB_KEY_F2,          TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},

    {"\x1b[1;2R",    TB_KEY_F3,          TB_MOD_SHIFT                           },
    {"\x1b[1;3R",    TB_KEY_F3,          TB_MOD_ALT                             },
    {"\x1b[1;4R",    TB_KEY_F3,          TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[1;5R",    TB_KEY_F3,          TB_MOD_CTRL                            },
    {"\x1b[1;6R",    TB_KEY_F3,          TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[1;7R",    TB_KEY_F3,          TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b[1;8R",    TB_KEY_F3,          TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},

    {"\x1b[1;2S",    TB_KEY_F4,          TB_MOD_SHIFT                           },
    {"\x1b[1;3S",    TB_KEY_F4,          TB_MOD_ALT                             },
    {"\x1b[1;4S",    TB_KEY_F4,          TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[1;5S",    TB_KEY_F4,          TB_MOD_CTRL                            },
    {"\x1b[1;6S",    TB_KEY_F4,          TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[1;7S",    TB_KEY_F4,          TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b[1;8S",    TB_KEY_F4,          TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},

    {"\x1b[15;2~",   TB_KEY_F5,          TB_MOD_SHIFT                           },
    {"\x1b[15;3~",   TB_KEY_F5,          TB_MOD_ALT                             },
    {"\x1b[15;4~",   TB_KEY_F5,          TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[15;5~",   TB_KEY_F5,          TB_MOD_CTRL                            },
    {"\x1b[15;6~",   TB_KEY_F5,          TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[15;7~",   TB_KEY_F5,          TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b[15;8~",   TB_KEY_F5,          TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},

    {"\x1b[17;2~",   TB_KEY_F6,          TB_MOD_SHIFT                           },
    {"\x1b[17;3~",   TB_KEY_F6,          TB_MOD_ALT                             },
    {"\x1b[17;4~",   TB_KEY_F6,          TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[17;5~",   TB_KEY_F6,          TB_MOD_CTRL                            },
    {"\x1b[17;6~",   TB_KEY_F6,          TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[17;7~",   TB_KEY_F6,          TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b[17;8~",   TB_KEY_F6,          TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},

    {"\x1b[18;2~",   TB_KEY_F7,          TB_MOD_SHIFT                           },
    {"\x1b[18;3~",   TB_KEY_F7,          TB_MOD_ALT                             },
    {"\x1b[18;4~",   TB_KEY_F7,          TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[18;5~",   TB_KEY_F7,          TB_MOD_CTRL                            },
    {"\x1b[18;6~",   TB_KEY_F7,          TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[18;7~",   TB_KEY_F7,          TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b[18;8~",   TB_KEY_F7,          TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},

    {"\x1b[19;2~",   TB_KEY_F8,          TB_MOD_SHIFT                           },
    {"\x1b[19;3~",   TB_KEY_F8,          TB_MOD_ALT                             },
    {"\x1b[19;4~",   TB_KEY_F8,          TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[19;5~",   TB_KEY_F8,          TB_MOD_CTRL                            },
    {"\x1b[19;6~",   TB_KEY_F8,          TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[19;7~",   TB_KEY_F8,          TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b[19;8~",   TB_KEY_F8,          TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},

    {"\x1b[20;2~",   TB_KEY_F9,          TB_MOD_SHIFT                           },
    {"\x1b[20;3~",   TB_KEY_F9,          TB_MOD_ALT                             },
    {"\x1b[20;4~",   TB_KEY_F9,          TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[20;5~",   TB_KEY_F9,          TB_MOD_CTRL                            },
    {"\x1b[20;6~",   TB_KEY_F9,          TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[20;7~",   TB_KEY_F9,          TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b[20;8~",   TB_KEY_F9,          TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},

    {"\x1b[21;2~",   TB_KEY_F10,         TB_MOD_SHIFT                           },
    {"\x1b[21;3~",   TB_KEY_F10,         TB_MOD_ALT                             },
    {"\x1b[21;4~",   TB_KEY_F10,         TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[21;5~",   TB_KEY_F10,         TB_MOD_CTRL                            },
    {"\x1b[21;6~",   TB_KEY_F10,         TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[21;7~",   TB_KEY_F10,         TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b[21;8~",   TB_KEY_F10,         TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},

    {"\x1b[23;2~",   TB_KEY_F11,         TB_MOD_SHIFT                           },
    {"\x1b[23;3~",   TB_KEY_F11,         TB_MOD_ALT                             },
    {"\x1b[23;4~",   TB_KEY_F11,         TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[23;5~",   TB_KEY_F11,         TB_MOD_CTRL                            },
    {"\x1b[23;6~",   TB_KEY_F11,         TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[23;7~",   TB_KEY_F11,         TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b[23;8~",   TB_KEY_F11,         TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},

    {"\x1b[24;2~",   TB_KEY_F12,         TB_MOD_SHIFT                           },
    {"\x1b[24;3~",   TB_KEY_F12,         TB_MOD_ALT                             },
    {"\x1b[24;4~",   TB_KEY_F12,         TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[24;5~",   TB_KEY_F12,         TB_MOD_CTRL                            },
    {"\x1b[24;6~",   TB_KEY_F12,         TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[24;7~",   TB_KEY_F12,         TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b[24;8~",   TB_KEY_F12,         TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},

 // rxvt arrows
    {"\x1b[a",       TB_KEY_ARROW_UP,    TB_MOD_SHIFT                           },
    {"\x1b\x1b[A",   TB_KEY_ARROW_UP,    TB_MOD_ALT                             },
    {"\x1b\x1b[a",   TB_KEY_ARROW_UP,    TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1bOa",       TB_KEY_ARROW_UP,    TB_MOD_CTRL                            },
    {"\x1b\x1bOa",   TB_KEY_ARROW_UP,    TB_MOD_CTRL | TB_MOD_ALT               },

    {"\x1b[b",       TB_KEY_ARROW_DOWN,  TB_MOD_SHIFT                           },
    {"\x1b\x1b[B",   TB_KEY_ARROW_DOWN,  TB_MOD_ALT                             },
    {"\x1b\x1b[b",   TB_KEY_ARROW_DOWN,  TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1bOb",       TB_KEY_ARROW_DOWN,  TB_MOD_CTRL                            },
    {"\x1b\x1bOb",   TB_KEY_ARROW_DOWN,  TB_MOD_CTRL | TB_MOD_ALT               },

    {"\x1b[c",       TB_KEY_ARROW_RIGHT, TB_MOD_SHIFT                           },
    {"\x1b\x1b[C",   TB_KEY_ARROW_RIGHT, TB_MOD_ALT                             },
    {"\x1b\x1b[c",   TB_KEY_ARROW_RIGHT, TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1bOc",       TB_KEY_ARROW_RIGHT, TB_MOD_CTRL                            },
    {"\x1b\x1bOc",   TB_KEY_ARROW_RIGHT, TB_MOD_CTRL | TB_MOD_ALT               },

    {"\x1b[d",       TB_KEY_ARROW_LEFT,  TB_MOD_SHIFT                           },
    {"\x1b\x1b[D",   TB_KEY_ARROW_LEFT,  TB_MOD_ALT                             },
    {"\x1b\x1b[d",   TB_KEY_ARROW_LEFT,  TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1bOd",       TB_KEY_ARROW_LEFT,  TB_MOD_CTRL                            },
    {"\x1b\x1bOd",   TB_KEY_ARROW_LEFT,  TB_MOD_CTRL | TB_MOD_ALT               },

 // rxvt keys
    {"\x1b[7$",      TB_KEY_HOME,        TB_MOD_SHIFT                           },
    {"\x1b\x1b[7~",  TB_KEY_HOME,        TB_MOD_ALT                             },
    {"\x1b\x1b[7$",  TB_KEY_HOME,        TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[7^",      TB_KEY_HOME,        TB_MOD_CTRL                            },
    {"\x1b[7@",      TB_KEY_HOME,        TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b\x1b[7^",  TB_KEY_HOME,        TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b\x1b[7@",  TB_KEY_HOME,        TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},

    {"\x1b\x1b[8~",  TB_KEY_END,         TB_MOD_ALT                             },
    {"\x1b\x1b[8$",  TB_KEY_END,         TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[8^",      TB_KEY_END,         TB_MOD_CTRL                            },
    {"\x1b\x1b[8^",  TB_KEY_END,         TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b\x1b[8@",  TB_KEY_END,         TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},
    {"\x1b[8@",      TB_KEY_END,         TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[8$",      TB_KEY_END,         TB_MOD_SHIFT                           },

    {"\x1b\x1b[2~",  TB_KEY_INSERT,      TB_MOD_ALT                             },
    {"\x1b\x1b[2$",  TB_KEY_INSERT,      TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[2^",      TB_KEY_INSERT,      TB_MOD_CTRL                            },
    {"\x1b\x1b[2^",  TB_KEY_INSERT,      TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b\x1b[2@",  TB_KEY_INSERT,      TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},
    {"\x1b[2@",      TB_KEY_INSERT,      TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[2$",      TB_KEY_INSERT,      TB_MOD_SHIFT                           },

    {"\x1b\x1b[3~",  TB_KEY_DELETE,      TB_MOD_ALT                             },
    {"\x1b\x1b[3$",  TB_KEY_DELETE,      TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[3^",      TB_KEY_DELETE,      TB_MOD_CTRL                            },
    {"\x1b\x1b[3^",  TB_KEY_DELETE,      TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b\x1b[3@",  TB_KEY_DELETE,      TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},
    {"\x1b[3@",      TB_KEY_DELETE,      TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[3$",      TB_KEY_DELETE,      TB_MOD_SHIFT                           },

    {"\x1b\x1b[5~",  TB_KEY_PGUP,        TB_MOD_ALT                             },
    {"\x1b\x1b[5$",  TB_KEY_PGUP,        TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[5^",      TB_KEY_PGUP,        TB_MOD_CTRL                            },
    {"\x1b\x1b[5^",  TB_KEY_PGUP,        TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b\x1b[5@",  TB_KEY_PGUP,        TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},
    {"\x1b[5@",      TB_KEY_PGUP,        TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[5$",      TB_KEY_PGUP,        TB_MOD_SHIFT                           },

    {"\x1b\x1b[6~",  TB_KEY_PGDN,        TB_MOD_ALT                             },
    {"\x1b\x1b[6$",  TB_KEY_PGDN,        TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[6^",      TB_KEY_PGDN,        TB_MOD_CTRL                            },
    {"\x1b\x1b[6^",  TB_KEY_PGDN,        TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b\x1b[6@",  TB_KEY_PGDN,        TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},
    {"\x1b[6@",      TB_KEY_PGDN,        TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[6$",      TB_KEY_PGDN,        TB_MOD_SHIFT                           },

    {"\x1b\x1b[11~", TB_KEY_F1,          TB_MOD_ALT                             },
    {"\x1b\x1b[23~", TB_KEY_F1,          TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[11^",     TB_KEY_F1,          TB_MOD_CTRL                            },
    {"\x1b\x1b[11^", TB_KEY_F1,          TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b\x1b[23^", TB_KEY_F1,          TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},
    {"\x1b[23^",     TB_KEY_F1,          TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[23~",     TB_KEY_F1,          TB_MOD_SHIFT                           },

    {"\x1b\x1b[12~", TB_KEY_F2,          TB_MOD_ALT                             },
    {"\x1b\x1b[24~", TB_KEY_F2,          TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[12^",     TB_KEY_F2,          TB_MOD_CTRL                            },
    {"\x1b\x1b[12^", TB_KEY_F2,          TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b\x1b[24^", TB_KEY_F2,          TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},
    {"\x1b[24^",     TB_KEY_F2,          TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[24~",     TB_KEY_F2,          TB_MOD_SHIFT                           },

    {"\x1b\x1b[13~", TB_KEY_F3,          TB_MOD_ALT                             },
    {"\x1b\x1b[25~", TB_KEY_F3,          TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[13^",     TB_KEY_F3,          TB_MOD_CTRL                            },
    {"\x1b\x1b[13^", TB_KEY_F3,          TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b\x1b[25^", TB_KEY_F3,          TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},
    {"\x1b[25^",     TB_KEY_F3,          TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[25~",     TB_KEY_F3,          TB_MOD_SHIFT                           },

    {"\x1b\x1b[14~", TB_KEY_F4,          TB_MOD_ALT                             },
    {"\x1b\x1b[26~", TB_KEY_F4,          TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[14^",     TB_KEY_F4,          TB_MOD_CTRL                            },
    {"\x1b\x1b[14^", TB_KEY_F4,          TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b\x1b[26^", TB_KEY_F4,          TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},
    {"\x1b[26^",     TB_KEY_F4,          TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[26~",     TB_KEY_F4,          TB_MOD_SHIFT                           },

    {"\x1b\x1b[15~", TB_KEY_F5,          TB_MOD_ALT                             },
    {"\x1b\x1b[28~", TB_KEY_F5,          TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[15^",     TB_KEY_F5,          TB_MOD_CTRL                            },
    {"\x1b\x1b[15^", TB_KEY_F5,          TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b\x1b[28^", TB_KEY_F5,          TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},
    {"\x1b[28^",     TB_KEY_F5,          TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[28~",     TB_KEY_F5,          TB_MOD_SHIFT                           },

    {"\x1b\x1b[17~", TB_KEY_F6,          TB_MOD_ALT                             },
    {"\x1b\x1b[29~", TB_KEY_F6,          TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[17^",     TB_KEY_F6,          TB_MOD_CTRL                            },
    {"\x1b\x1b[17^", TB_KEY_F6,          TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b\x1b[29^", TB_KEY_F6,          TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},
    {"\x1b[29^",     TB_KEY_F6,          TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[29~",     TB_KEY_F6,          TB_MOD_SHIFT                           },

    {"\x1b\x1b[18~", TB_KEY_F7,          TB_MOD_ALT                             },
    {"\x1b\x1b[31~", TB_KEY_F7,          TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[18^",     TB_KEY_F7,          TB_MOD_CTRL                            },
    {"\x1b\x1b[18^", TB_KEY_F7,          TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b\x1b[31^", TB_KEY_F7,          TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},
    {"\x1b[31^",     TB_KEY_F7,          TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[31~",     TB_KEY_F7,          TB_MOD_SHIFT                           },

    {"\x1b\x1b[19~", TB_KEY_F8,          TB_MOD_ALT                             },
    {"\x1b\x1b[32~", TB_KEY_F8,          TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[19^",     TB_KEY_F8,          TB_MOD_CTRL                            },
    {"\x1b\x1b[19^", TB_KEY_F8,          TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b\x1b[32^", TB_KEY_F8,          TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},
    {"\x1b[32^",     TB_KEY_F8,          TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[32~",     TB_KEY_F8,          TB_MOD_SHIFT                           },

    {"\x1b\x1b[20~", TB_KEY_F9,          TB_MOD_ALT                             },
    {"\x1b\x1b[33~", TB_KEY_F9,          TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[20^",     TB_KEY_F9,          TB_MOD_CTRL                            },
    {"\x1b\x1b[20^", TB_KEY_F9,          TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b\x1b[33^", TB_KEY_F9,          TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},
    {"\x1b[33^",     TB_KEY_F9,          TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[33~",     TB_KEY_F9,          TB_MOD_SHIFT                           },

    {"\x1b\x1b[21~", TB_KEY_F10,         TB_MOD_ALT                             },
    {"\x1b\x1b[34~", TB_KEY_F10,         TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[21^",     TB_KEY_F10,         TB_MOD_CTRL                            },
    {"\x1b\x1b[21^", TB_KEY_F10,         TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b\x1b[34^", TB_KEY_F10,         TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},
    {"\x1b[34^",     TB_KEY_F10,         TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[34~",     TB_KEY_F10,         TB_MOD_SHIFT                           },

    {"\x1b\x1b[23~", TB_KEY_F11,         TB_MOD_ALT                             },
    {"\x1b\x1b[23$", TB_KEY_F11,         TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[23^",     TB_KEY_F11,         TB_MOD_CTRL                            },
    {"\x1b\x1b[23^", TB_KEY_F11,         TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b\x1b[23@", TB_KEY_F11,         TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},
    {"\x1b[23@",     TB_KEY_F11,         TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[23$",     TB_KEY_F11,         TB_MOD_SHIFT                           },

    {"\x1b\x1b[24~", TB_KEY_F12,         TB_MOD_ALT                             },
    {"\x1b\x1b[24$", TB_KEY_F12,         TB_MOD_ALT | TB_MOD_SHIFT              },
    {"\x1b[24^",     TB_KEY_F12,         TB_MOD_CTRL                            },
    {"\x1b\x1b[24^", TB_KEY_F12,         TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1b\x1b[24@", TB_KEY_F12,         TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT},
    {"\x1b[24@",     TB_KEY_F12,         TB_MOD_CTRL | TB_MOD_SHIFT             },
    {"\x1b[24$",     TB_KEY_F12,         TB_MOD_SHIFT                           },

 // linux console/putty arrows
    {"\x1b[A",       TB_KEY_ARROW_UP,    TB_MOD_SHIFT                           },
    {"\x1b[B",       TB_KEY_ARROW_DOWN,  TB_MOD_SHIFT                           },
    {"\x1b[C",       TB_KEY_ARROW_RIGHT, TB_MOD_SHIFT                           },
    {"\x1b[D",       TB_KEY_ARROW_LEFT,  TB_MOD_SHIFT                           },

 // more putty arrows
    {"\x1bOA",       TB_KEY_ARROW_UP,    TB_MOD_CTRL                            },
    {"\x1b\x1bOA",   TB_KEY_ARROW_UP,    TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1bOB",       TB_KEY_ARROW_DOWN,  TB_MOD_CTRL                            },
    {"\x1b\x1bOB",   TB_KEY_ARROW_DOWN,  TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1bOC",       TB_KEY_ARROW_RIGHT, TB_MOD_CTRL                            },
    {"\x1b\x1bOC",   TB_KEY_ARROW_RIGHT, TB_MOD_CTRL | TB_MOD_ALT               },
    {"\x1bOD",       TB_KEY_ARROW_LEFT,  TB_MOD_CTRL                            },
    {"\x1b\x1bOD",   TB_KEY_ARROW_LEFT,  TB_MOD_CTRL | TB_MOD_ALT               },

    {NULL,           0,                  0                                      },
};

static const unsigned char utf8_length[256] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1};

static const unsigned char utf8_mask[6] = {0x7f, 0x1f, 0x0f, 0x07, 0x03, 0x01};

static int tb_reset();
static int tb_printf_inner(int x, int y, uintattr_t fg, uintattr_t bg,
    size_t *out_w, const char *fmt, va_list vl);
static int init_term_attrs();
static int init_term_caps();
static int init_cap_trie();
static int cap_trie_add(const char *cap, uint16_t key, uint8_t mod);
static int cap_trie_find(const char *buf, size_t nbuf, struct cap_trie_t **last,
    size_t *depth);
static int cap_trie_deinit(struct cap_trie_t *node);
static int init_resize_handler();
static int send_init_escape_codes();
static int send_clear();
static int update_term_size();
static int update_term_size_via_esc();
static int init_cellbuf();
static int tb_deinit();
static int load_terminfo();
static int load_terminfo_from_path(const char *path, const char *term);
static int read_terminfo_path(const char *path);
static int parse_terminfo_caps();
static int load_builtin_caps();
static const char *get_terminfo_string(int16_t str_offsets_pos,
    int16_t str_table_pos, int16_t str_table_len, int16_t str_index);
static int wait_event(struct tb_event *event, int timeout);
static int extract_event(struct tb_event *event);
static int extract_esc(struct tb_event *event);
static int extract_esc_user(struct tb_event *event, int is_post);
static int extract_esc_cap(struct tb_event *event);
static int extract_esc_mouse(struct tb_event *event);
static int resize_if_needed();
static void handle_resize(int sig);
static int send_attr(uintattr_t fg, uintattr_t bg);
static int send_sgr(uintattr_t fg, uintattr_t bg);
static int send_cursor_if(int x, int y);
static int send_char(int x, int y, uint32_t ch);
static int send_cluster(int x, int y, uint32_t *ch, size_t nch);
static int convert_num(uint32_t num, char *buf);
static int cell_cmp(struct tb_cell *a, struct tb_cell *b);
static int cell_copy(struct tb_cell *dst, struct tb_cell *src);
static int cell_set(struct tb_cell *cell, uint32_t *ch, size_t nch,
    uintattr_t fg, uintattr_t bg);
static int cell_reserve_ech(struct tb_cell *cell, size_t n);
static int cell_free(struct tb_cell *cell);
static int cellbuf_init(struct cellbuf_t *c, int w, int h);
static int cellbuf_free(struct cellbuf_t *c);
static int cellbuf_clear(struct cellbuf_t *c);
static int cellbuf_get(struct cellbuf_t *c, int x, int y, struct tb_cell **out);
static int cellbuf_resize(struct cellbuf_t *c, int w, int h);
static int bytebuf_puts(struct bytebuf_t *b, const char *str);
static int bytebuf_nputs(struct bytebuf_t *b, const char *str, size_t nstr);
static int bytebuf_shift(struct bytebuf_t *b, size_t n);
static int bytebuf_flush(struct bytebuf_t *b, int fd);
static int bytebuf_reserve(struct bytebuf_t *b, size_t sz);
static int bytebuf_free(struct bytebuf_t *b);

int tb_init() {
    return tb_init_file("/dev/tty");
}

int tb_init_file(const char *path) {
    if (global.initialized) {
        return TB_ERR_INIT_ALREADY;
    }
    int ttyfd = open(path, O_RDWR);
    if (ttyfd < 0) {
        global.last_errno = errno;
        return TB_ERR_INIT_OPEN;
    }
    global.ttyfd_open = 1;
    return tb_init_fd(ttyfd);
}

int tb_init_fd(int ttyfd) {
    return tb_init_rwfd(ttyfd, ttyfd);
}

int tb_init_rwfd(int rfd, int wfd) {
    int rv;

    tb_reset();
    global.ttyfd = rfd == wfd && isatty(rfd) ? rfd : -1;
    global.rfd = rfd;
    global.wfd = wfd;

    do {
        if_err_break(rv, init_term_attrs());
        if_err_break(rv, init_term_caps());
        if_err_break(rv, init_cap_trie());
        if_err_break(rv, init_resize_handler());
        if_err_break(rv, send_init_escape_codes());
        if_err_break(rv, send_clear());
        if_err_break(rv, update_term_size());
        if_err_break(rv, init_cellbuf());
        global.initialized = 1;
    } while (0);

    if (rv != TB_OK) {
        tb_deinit();
    }

    return rv;
}

int tb_shutdown() {
    if_not_init_return();
    tb_deinit();
    return TB_OK;
}

int tb_width() {
    if_not_init_return();
    return global.width;
}

int tb_height() {
    if_not_init_return();
    return global.height;
}

int tb_clear() {
    int rv;
    if_not_init_return();
    if_err_return(rv, resize_if_needed());
    return cellbuf_clear(&global.back);
}

int tb_set_clear_attrs(uintattr_t fg, uintattr_t bg) {
    if_not_init_return();
    global.fg = fg;
    global.bg = bg;
    return TB_OK;
}

int tb_present() {
    if_not_init_return();

    int rv;
    if_err_return(rv, resize_if_needed());

    // TODO Assert global.back.(width,height) == global.front.(width,height)

    global.last_x = -1;
    global.last_y = -1;

    int x, y, i;
    for (y = 0; y < global.front.height; y++) {
        for (x = 0; x < global.front.width;) {
            struct tb_cell *back, *front;
            if_err_return(rv, cellbuf_get(&global.back, x, y, &back));
            if_err_return(rv, cellbuf_get(&global.front, x, y, &front));

            int w;
            {
#ifdef TB_OPT_EGC
                if (back->nech > 0)
                    w = wcswidth((wchar_t *)back->ech, back->nech);
                else
#endif
                    w = wcwidth(back->ch);
            }
            if (w < 1) {
                w = 1;
            }

            if (cell_cmp(back, front) != 0) {
                cell_copy(front, back);

                send_attr(back->fg, back->bg);
                if (w > 1 && x >= global.front.width - (w - 1)) {
                    for (i = x; i < global.front.width; i++) {
                        send_char(i, y, ' ');
                    }
                } else {
                    {
#ifdef TB_OPT_EGC
                        if (back->nech > 0)
                            send_cluster(x, y, back->ech, back->nech);
                        else
#endif
                            send_char(x, y, back->ch);
                    }
                    for (i = 1; i < w; i++) {
                        struct tb_cell *front_wide;
                        if_err_return(rv,
                            cellbuf_get(&global.front, x + i, y, &front_wide));
                        cell_set(front_wide, 0, 1, back->fg, back->bg);
                    }
                }
            }
            x += w;
        }
    }

    if_err_return(rv, send_cursor_if(global.cursor_x, global.cursor_y));
    if_err_return(rv, bytebuf_flush(&global.out, global.wfd));

    return TB_OK;
}

int tb_set_cursor(int cx, int cy) {
    if_not_init_return();
    int rv;
    if (cx < 0)
        cx = 0;
    if (cy < 0)
        cy = 0;
    if (global.cursor_x == -1) {
        if_err_return(rv,
            bytebuf_puts(&global.out, global.caps[TB_CAP_SHOW_CURSOR]));
    }
    if_err_return(rv, send_cursor_if(cx, cy));
    global.cursor_x = cx;
    global.cursor_y = cy;
    return TB_OK;
}

int tb_hide_cursor() {
    if_not_init_return();
    int rv;
    if (global.cursor_x >= 0) {
        if_err_return(rv,
            bytebuf_puts(&global.out, global.caps[TB_CAP_HIDE_CURSOR]));
    }
    global.cursor_x = -1;
    global.cursor_y = -1;
    return TB_OK;
}

int tb_set_cell(int x, int y, uint32_t ch, uintattr_t fg, uintattr_t bg) {
    if_not_init_return();
    return tb_set_cell_ex(x, y, &ch, 1, fg, bg);
}

int tb_set_cell_ex(int x, int y, uint32_t *ch, size_t nch, uintattr_t fg,
    uintattr_t bg) {
    if_not_init_return();
    int rv;
    struct tb_cell *cell;
    if_err_return(rv, cellbuf_get(&global.back, x, y, &cell));
    if_err_return(rv, cell_set(cell, ch, nch, fg, bg));
    return TB_OK;
}

int tb_extend_cell(int x, int y, uint32_t ch) {
    if_not_init_return();
#ifdef TB_OPT_EGC
    int rv;
    struct tb_cell *cell;
    size_t nech;
    if_err_return(rv, cellbuf_get(&global.back, x, y, &cell));
    if (cell->nech > 0) { // append to ech
        nech = cell->nech + 1;
        if_err_return(rv, cell_reserve_ech(cell, nech));
        cell->ech[nech - 1] = ch;
    } else { // make new ech
        nech = 2;
        if_err_return(rv, cell_reserve_ech(cell, nech));
        cell->ech[0] = cell->ch;
        cell->ech[1] = ch;
    }
    cell->ech[nech] = '\0';
    cell->nech = nech;
    return TB_OK;
#else
    (void)x;
    (void)y;
    (void)ch;
    return TB_ERR;
#endif
}

int tb_set_input_mode(int mode) {
    if_not_init_return();
    if (mode == TB_INPUT_CURRENT) {
        return global.input_mode;
    }

    if ((mode & (TB_INPUT_ESC | TB_INPUT_ALT)) == 0) {
        mode |= TB_INPUT_ESC;
    }

    if ((mode & (TB_INPUT_ESC | TB_INPUT_ALT)) == (TB_INPUT_ESC | TB_INPUT_ALT))
    {
        mode &= ~TB_INPUT_ALT;
    }

    if (mode & TB_INPUT_MOUSE) {
        bytebuf_puts(&global.out, TB_HARDCAP_ENTER_MOUSE);
        bytebuf_flush(&global.out, global.wfd);
    } else {
        bytebuf_puts(&global.out, TB_HARDCAP_EXIT_MOUSE);
        bytebuf_flush(&global.out, global.wfd);
    }

    global.input_mode = mode;
    return TB_OK;
}

int tb_set_output_mode(int mode) {
    if_not_init_return();
    if (mode == TB_OUTPUT_CURRENT) {
        return global.output_mode;
    }
    global.output_mode = mode;
    return TB_OK;
}

int tb_peek_event(struct tb_event *event, int timeout_ms) {
    if_not_init_return();
    return wait_event(event, timeout_ms);
}

int tb_poll_event(struct tb_event *event) {
    if_not_init_return();
    return wait_event(event, -1);
}

int tb_get_fds(int *ttyfd, int *resizefd) {
    if_not_init_return();

    *ttyfd = global.rfd;
    *resizefd = global.resize_pipefd[0];

    return TB_OK;
}

int tb_print(int x, int y, uintattr_t fg, uintattr_t bg, const char *str) {
    return tb_print_ex(x, y, fg, bg, NULL, str);
}

int tb_print_ex(int x, int y, uintattr_t fg, uintattr_t bg, size_t *out_w,
    const char *str) {
    int rv;
    uint32_t uni;
    int w, ix = x;
    if (out_w) {
        *out_w = 0;
    }
    while (*str) {
        str += tb_utf8_char_to_unicode(&uni, str);
        w = wcwidth(uni);
        if (w < 0) {
            w = 1;
        }
        if (w == 0 && x > ix) {
            if_err_return(rv, tb_extend_cell(x - 1, y, uni));
        } else {
            if_err_return(rv, tb_set_cell(x, y, uni, fg, bg));
        }
        x += w;
        if (out_w) {
            *out_w += w;
        }
    }
    return TB_OK;
}

int tb_printf(int x, int y, uintattr_t fg, uintattr_t bg, const char *fmt,
    ...) {
    int rv;
    va_list vl;
    va_start(vl, fmt);
    rv = tb_printf_inner(x, y, fg, bg, NULL, fmt, vl);
    va_end(vl);
    return rv;
}

int tb_printf_ex(int x, int y, uintattr_t fg, uintattr_t bg, size_t *out_w,
    const char *fmt, ...) {
    int rv;
    va_list vl;
    va_start(vl, fmt);
    rv = tb_printf_inner(x, y, fg, bg, out_w, fmt, vl);
    va_end(vl);
    return rv;
}

int tb_send(const char *buf, size_t nbuf) {
    return bytebuf_nputs(&global.out, buf, nbuf);
}

int tb_sendf(const char *fmt, ...) {
    int rv;
    char buf[TB_OPT_PRINTF_BUF];
    va_list vl;
    va_start(vl, fmt);
    rv = vsnprintf(buf, sizeof(buf), fmt, vl);
    va_end(vl);
    if (rv < 0 || rv >= (int)sizeof(buf)) {
        return TB_ERR;
    }
    return tb_send(buf, (size_t)rv);
}

int tb_set_func(int fn_type, int (*fn)(struct tb_event *, size_t *)) {
    switch (fn_type) {
        case TB_FUNC_EXTRACT_PRE:
            global.fn_extract_esc_pre = fn;
            return TB_OK;
        case TB_FUNC_EXTRACT_POST:
            global.fn_extract_esc_post = fn;
            return TB_OK;
    }
    return TB_ERR;
}

struct tb_cell *tb_cell_buffer() {
    if (!global.initialized)
        return NULL;
    return global.back.cells;
}

int tb_utf8_char_length(char c) {
    return utf8_length[(unsigned char)c];
}

int tb_utf8_char_to_unicode(uint32_t *out, const char *c) {
    if (*c == 0) {
        return TB_ERR;
    }

    int i;
    unsigned char len = tb_utf8_char_length(*c);
    unsigned char mask = utf8_mask[len - 1];
    uint32_t result = c[0] & mask;
    for (i = 1; i < len; ++i) {
        result <<= 6;
        result |= c[i] & 0x3f;
    }

    *out = result;
    return (int)len;
}

int tb_utf8_unicode_to_char(char *out, uint32_t c) {
    int len = 0;
    int first;
    int i;

    if (c < 0x80) {
        first = 0;
        len = 1;
    } else if (c < 0x800) {
        first = 0xc0;
        len = 2;
    } else if (c < 0x10000) {
        first = 0xe0;
        len = 3;
    } else if (c < 0x200000) {
        first = 0xf0;
        len = 4;
    } else if (c < 0x4000000) {
        first = 0xf8;
        len = 5;
    } else {
        first = 0xfc;
        len = 6;
    }

    for (i = len - 1; i > 0; --i) {
        out[i] = (c & 0x3f) | 0x80;
        c >>= 6;
    }
    out[0] = c | first;

    return len;
}

int tb_last_errno() {
    return global.last_errno;
}

const char *tb_strerror(int err) {
    switch (err) {
        case TB_OK:
            return "Success";
        case TB_ERR_NEED_MORE:
            return "Not enough input";
        case TB_ERR_INIT_ALREADY:
            return "Termbox initialized already";
        case TB_ERR_MEM:
            return "Out of memory";
        case TB_ERR_NO_EVENT:
            return "No event";
        case TB_ERR_NO_TERM:
            return "No TERM in environment";
        case TB_ERR_NOT_INIT:
            return "Termbox not initialized";
        case TB_ERR_OUT_OF_BOUNDS:
            return "Out of bounds";
        case TB_ERR_UNSUPPORTED_TERM:
            return "Unsupported terminal";
        case TB_ERR_CAP_COLLISION:
            return "Termcaps collision";
        case TB_ERR_RESIZE_SSCANF:
            return "Terminal width/height not received by sscanf() after "
                   "resize";
        case TB_ERR:
        case TB_ERR_INIT_OPEN:
        case TB_ERR_READ:
        case TB_ERR_RESIZE_IOCTL:
        case TB_ERR_RESIZE_PIPE:
        case TB_ERR_RESIZE_SIGACTION:
        case TB_ERR_POLL:
        case TB_ERR_TCGETATTR:
        case TB_ERR_TCSETATTR:
        case TB_ERR_RESIZE_WRITE:
        case TB_ERR_RESIZE_POLL:
        case TB_ERR_RESIZE_READ:
        default:
            return strerror(global.last_errno);
    }
}

static int tb_reset() {
    int ttyfd_open = global.ttyfd_open;
    memset(&global, 0, sizeof(global));
    global.ttyfd = -1;
    global.rfd = -1;
    global.wfd = -1;
    global.ttyfd_open = ttyfd_open;
    global.resize_pipefd[0] = -1;
    global.resize_pipefd[1] = -1;
    global.width = -1;
    global.height = -1;
    global.cursor_x = -1;
    global.cursor_y = -1;
    global.last_x = -1;
    global.last_y = -1;
    global.fg = TB_DEFAULT;
    global.bg = TB_DEFAULT;
    global.last_fg = ~global.fg;
    global.last_bg = ~global.bg;
    global.input_mode = TB_INPUT_ESC;
    global.output_mode = TB_OUTPUT_NORMAL;
    return TB_OK;
}

static int init_term_attrs() {
    if (global.ttyfd < 0) {
        return TB_OK;
    }

    if (tcgetattr(global.ttyfd, &global.orig_tios) != 0) {
        global.last_errno = errno;
        return TB_ERR_TCGETATTR;
    }

    struct termios tios;
    memcpy(&tios, &global.orig_tios, sizeof(tios));
    global.has_orig_tios = 1;

    cfmakeraw(&tios);
    tios.c_cc[VMIN] = 1;
    tios.c_cc[VTIME] = 0;

    if (tcsetattr(global.ttyfd, TCSAFLUSH, &tios) != 0) {
        global.last_errno = errno;
        return TB_ERR_TCSETATTR;
    }

    return TB_OK;
}

int tb_printf_inner(int x, int y, uintattr_t fg, uintattr_t bg, size_t *out_w,
    const char *fmt, va_list vl) {
    int rv;
    char buf[TB_OPT_PRINTF_BUF];
    rv = vsnprintf(buf, sizeof(buf), fmt, vl);
    if (rv < 0 || rv >= (int)sizeof(buf)) {
        return TB_ERR;
    }
    return tb_print_ex(x, y, fg, bg, out_w, buf);
}

static int init_term_caps() {
    if (load_terminfo() == TB_OK) {
        return parse_terminfo_caps();
    }
    return load_builtin_caps();
}

static int init_cap_trie() {
    int rv, i;

    // Add caps from terminfo or built-in
    for (i = 0; i < TB_CAP__COUNT_KEYS; i++) {
        if_err_return(rv, cap_trie_add(global.caps[i], tb_key_i(i), 0));
    }

    // Add built-in mod caps
    for (i = 0; builtin_mod_caps[i].cap != NULL; i++) {
        rv = cap_trie_add(builtin_mod_caps[i].cap, builtin_mod_caps[i].key,
            builtin_mod_caps[i].mod);
        // Collisions are OK. This can happen if global.caps collides with
        // builtin_mod_caps. It is desirable to give precedence to global.caps
        // here.
        if (rv != TB_OK && rv != TB_ERR_CAP_COLLISION) {
            return rv;
        }
    }

    return TB_OK;
}

static int cap_trie_add(const char *cap, uint16_t key, uint8_t mod) {
    struct cap_trie_t *next, *node = &global.cap_trie;
    size_t i, j;
    for (i = 0; cap[i] != '\0'; i++) {
        char c = cap[i];
        next = NULL;

        // Check if c is already a child of node
        for (j = 0; j < node->nchildren; j++) {
            if (node->children[j].c == c) {
                next = &node->children[j];
                break;
            }
        }
        if (!next) {
            // We need to add a new child to node
            node->nchildren += 1;
            node->children =
                tb_realloc(node->children, sizeof(*node) * node->nchildren);
            if (!node->children) {
                return TB_ERR_MEM;
            }
            next = &node->children[node->nchildren - 1];
            memset(next, 0, sizeof(*next));
            next->c = c;
        }

        // Continue
        node = next;
    }

    if (node->is_leaf) {
        // Already a leaf here
        return TB_ERR_CAP_COLLISION;
    }

    node->is_leaf = 1;
    node->key = key;
    node->mod = mod;
    return TB_OK;
}

static int cap_trie_find(const char *buf, size_t nbuf, struct cap_trie_t **last,
    size_t *depth) {
    struct cap_trie_t *next, *node = &global.cap_trie;
    size_t i, j;
    *last = node;
    *depth = 0;
    for (i = 0; i < nbuf; i++) {
        char c = buf[i];
        next = NULL;

        // Find c in node.children
        for (j = 0; j < node->nchildren; j++) {
            if (node->children[j].c == c) {
                next = &node->children[j];
                break;
            }
        }
        if (!next) {
            // Not found
            return TB_OK;
        }
        node = next;
        *last = node;
        *depth += 1;
        if (node->is_leaf && node->nchildren < 1) {
            break;
        }
    }
    return TB_OK;
}

static int cap_trie_deinit(struct cap_trie_t *node) {
    size_t j;
    for (j = 0; j < node->nchildren; j++) {
        cap_trie_deinit(&node->children[j]);
    }
    if (node->children) {
        tb_free(node->children);
    }
    memset(node, 0, sizeof(*node));
    return TB_OK;
}

static int init_resize_handler() {
    if (pipe(global.resize_pipefd) != 0) {
        global.last_errno = errno;
        return TB_ERR_RESIZE_PIPE;
    }

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_resize;
    if (sigaction(SIGWINCH, &sa, NULL) != 0) {
        global.last_errno = errno;
        return TB_ERR_RESIZE_SIGACTION;
    }

    return TB_OK;
}

static int send_init_escape_codes() {
    int rv;
    if_err_return(rv, bytebuf_puts(&global.out, global.caps[TB_CAP_ENTER_CA]));
    if_err_return(rv,
        bytebuf_puts(&global.out, global.caps[TB_CAP_ENTER_KEYPAD]));
    if_err_return(rv,
        bytebuf_puts(&global.out, global.caps[TB_CAP_HIDE_CURSOR]));
    return TB_OK;
}

static int send_clear() {
    int rv;

    if_err_return(rv, send_attr(global.fg, global.bg));
    if_err_return(rv,
        bytebuf_puts(&global.out, global.caps[TB_CAP_CLEAR_SCREEN]));

    if_err_return(rv, send_cursor_if(global.cursor_x, global.cursor_y));
    if_err_return(rv, bytebuf_flush(&global.out, global.wfd));

    global.last_x = -1;
    global.last_y = -1;

    return TB_OK;
}

static int update_term_size() {
    int rv;

    if (global.ttyfd < 0) {
        return TB_OK;
    }

    struct winsize sz;
    memset(&sz, 0, sizeof(sz));

    if (ioctl(global.ttyfd, TIOCGWINSZ, &sz) == 0) {
        global.width = sz.ws_col;
        global.height = sz.ws_row;
        return TB_OK;
    }

    // If TB_RESIZE_FALLBACK deinfed, try >cursor(9999,9999), >u7, <u6
    //#ifdef TB_RESIZE_FALLBACK
    if_ok_return(rv, update_term_size_via_esc());
    //#endif

    global.last_errno = errno;
    return TB_ERR_RESIZE_IOCTL;
}

static int update_term_size_via_esc() {
#ifndef TB_RESIZE_FALLBACK_MS
#define TB_RESIZE_FALLBACK_MS 1000
#endif

    char *move_and_report = "\x1b[9999;9999H\x1b[6n";
    ssize_t write_rv =
        write(global.wfd, move_and_report, strlen(move_and_report));
    if (write_rv != (ssize_t)strlen(move_and_report)) {
        return TB_ERR_RESIZE_WRITE;
    }

#if !TB_OPT_SELECT
    struct pollfd fd = {.fd = global.rfd, .events = POLLIN};
    int poll_rv = poll(&fd, 1, TB_RESIZE_FALLBACK_MS);
#else
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(global.rfd, &fds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = TB_RESIZE_FALLBACK_MS * 1000;

    int poll_rv = select(global.rfd + 1, &fds, NULL, NULL, &timeout);
#endif

    if (poll_rv != 1) {
        global.last_errno = errno;
        return TB_ERR_RESIZE_POLL;
    }

    char buf[TB_OPT_READ_BUF];
    ssize_t read_rv = read(global.rfd, buf, sizeof(buf) - 1);
    if (read_rv < 1) {
        global.last_errno = errno;
        return TB_ERR_RESIZE_READ;
    }
    buf[read_rv] = '\0';

    int rw, rh;
    if (sscanf(buf, "\x1b[%d;%dR", &rh, &rw) != 2) {
        return TB_ERR_RESIZE_SSCANF;
    }

    global.width = rw;
    global.height = rh;
    return TB_OK;
}

static int init_cellbuf() {
    int rv;
    if_err_return(rv, cellbuf_init(&global.back, global.width, global.height));
    if_err_return(rv, cellbuf_init(&global.front, global.width, global.height));
    if_err_return(rv, cellbuf_clear(&global.back));
    if_err_return(rv, cellbuf_clear(&global.front));
    return TB_OK;
}

static int tb_deinit() {
    if (global.caps[0] != NULL && global.wfd >= 0) {
        bytebuf_puts(&global.out, global.caps[TB_CAP_SHOW_CURSOR]);
        bytebuf_puts(&global.out, global.caps[TB_CAP_SGR0]);
        bytebuf_puts(&global.out, global.caps[TB_CAP_CLEAR_SCREEN]);
        bytebuf_puts(&global.out, global.caps[TB_CAP_EXIT_CA]);
        bytebuf_puts(&global.out, global.caps[TB_CAP_EXIT_KEYPAD]);
        bytebuf_puts(&global.out, TB_HARDCAP_EXIT_MOUSE);
        bytebuf_flush(&global.out, global.wfd);
    }
    if (global.ttyfd >= 0) {
        if (global.has_orig_tios) {
            tcsetattr(global.ttyfd, TCSAFLUSH, &global.orig_tios);
        }
        if (global.ttyfd_open) {
            close(global.ttyfd);
            global.ttyfd_open = 0;
        }
    }

    sigaction(SIGWINCH, &(struct sigaction){.sa_handler = SIG_DFL}, NULL);
    if (global.resize_pipefd[0] >= 0)
        close(global.resize_pipefd[0]);
    if (global.resize_pipefd[1] >= 0)
        close(global.resize_pipefd[1]);

    cellbuf_free(&global.back);
    cellbuf_free(&global.front);
    bytebuf_free(&global.in);
    bytebuf_free(&global.out);

    if (global.terminfo)
        tb_free(global.terminfo);

    cap_trie_deinit(&global.cap_trie);

    tb_reset();
    return TB_OK;
}

static int load_terminfo() {
    int rv;
    char tmp[PATH_MAX];

    // See terminfo(5) "Fetching Compiled Descriptions" for a description of
    // this behavior. Some of these paths are compile-time ncurses options, so
    // best guesses are used here.
    const char *term = getenv("TERM");
    if (!term) {
        return TB_ERR;
    }

    // If TERMINFO is set, try that directory and stop
    const char *terminfo = getenv("TERMINFO");
    if (terminfo) {
        return load_terminfo_from_path(terminfo, term);
    }

    // Next try ~/.terminfo
    const char *home = getenv("HOME");
    if (home) {
        snprintf_or_return(rv, tmp, sizeof(tmp), "%s/.terminfo", home);
        if_ok_return(rv, load_terminfo_from_path(tmp, term));
    }

    // Next try TERMINFO_DIRS
    //
    // Note, empty entries are supposed to be interpretted as the "compiled-in
    // default", which is of course system-dependent. Previously /etc/terminfo
    // was used here. Let's skip empty entries altogether rather than give
    // precedence to a guess, and check common paths after this loop.
    const char *dirs = getenv("TERMINFO_DIRS");
    if (dirs) {
        snprintf_or_return(rv, tmp, sizeof(tmp), "%s", dirs);
        char *dir = strtok(tmp, ":");
        while (dir) {
            const char *cdir = dir;
            if (*cdir != '\0') {
                if_ok_return(rv, load_terminfo_from_path(cdir, term));
            }
            dir = strtok(NULL, ":");
        }
    }

#ifdef TB_TERMINFO_DIR
    if_ok_return(rv, load_terminfo_from_path(TB_TERMINFO_DIR, term));
#endif
    if_ok_return(rv, load_terminfo_from_path("/usr/local/etc/terminfo", term));
    if_ok_return(rv,
        load_terminfo_from_path("/usr/local/share/terminfo", term));
    if_ok_return(rv, load_terminfo_from_path("/usr/local/lib/terminfo", term));
    if_ok_return(rv, load_terminfo_from_path("/etc/terminfo", term));
    if_ok_return(rv, load_terminfo_from_path("/usr/share/terminfo", term));
    if_ok_return(rv, load_terminfo_from_path("/usr/lib/terminfo", term));
    if_ok_return(rv, load_terminfo_from_path("/usr/share/lib/terminfo", term));
    if_ok_return(rv, load_terminfo_from_path("/lib/terminfo", term));

    return TB_ERR;
}

static int load_terminfo_from_path(const char *path, const char *term) {
    int rv;
    char tmp[PATH_MAX];

    // Look for term at this terminfo location, e.g., <terminfo>/x/xterm
    snprintf_or_return(rv, tmp, sizeof(tmp), "%s/%c/%s", path, term[0], term);
    if_ok_return(rv, read_terminfo_path(tmp));

#ifdef __APPLE__
    // Try the Darwin equivalent path, e.g., <terminfo>/78/xterm
    snprintf_or_return(rv, tmp, sizeof(tmp), "%s/%x/%s", path, term[0], term);
    return read_terminfo_path(tmp);
#endif

    return TB_ERR;
}

static int read_terminfo_path(const char *path) {
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        return TB_ERR;
    }

    struct stat st;
    if (fstat(fileno(fp), &st) != 0) {
        fclose(fp);
        return TB_ERR;
    }

    size_t fsize = st.st_size;
    char *data = tb_malloc(fsize);
    if (!data) {
        fclose(fp);
        return TB_ERR;
    }

    if (fread(data, 1, fsize, fp) != fsize) {
        fclose(fp);
        tb_free(data);
        return TB_ERR;
    }

    global.terminfo = data;
    global.nterminfo = fsize;

    fclose(fp);
    return TB_OK;
}

static int parse_terminfo_caps() {
    // See term(5) "LEGACY STORAGE FORMAT" and "EXTENDED STORAGE FORMAT" for a
    // description of this behavior.

    // Ensure there's at least a header's worth of data
    if (global.nterminfo < 6) {
        return TB_ERR;
    }

    int16_t *header = (int16_t *)global.terminfo;
    // header[0] the magic number (octal 0432 or 01036)
    // header[1] the size, in bytes, of the names section
    // header[2] the number of bytes in the boolean section
    // header[3] the number of short integers in the numbers section
    // header[4] the number of offsets (short integers) in the strings section
    // header[5] the size, in bytes, of the string table

    // Legacy ints are 16-bit, extended ints are 32-bit
    const int bytes_per_int = header[0] == 01036 ? 4  // 32-bit
                                                 : 2; // 16-bit

    // > Between the boolean section and the number section, a null byte will be
    // > inserted, if necessary, to ensure that the number section begins on an
    // > even byte
    const int align_offset = (header[1] + header[2]) % 2 != 0 ? 1 : 0;

    const int pos_str_offsets =
        (6 * sizeof(int16_t)) // header (12 bytes)
        + header[1]           // length of names section
        + header[2]           // length of boolean section
        + align_offset +
        (header[3] * bytes_per_int); // length of numbers section

    const int pos_str_table =
        pos_str_offsets +
        (header[4] * sizeof(int16_t)); // length of string offsets table

    // Load caps
    int i;
    for (i = 0; i < TB_CAP__COUNT; i++) {
        const char *cap = get_terminfo_string(pos_str_offsets, pos_str_table,
            header[5], terminfo_cap_indexes[i]);
        if (!cap) {
            // Something is not right
            return TB_ERR;
        }
        global.caps[i] = cap;
    }

    return TB_OK;
}

static int load_builtin_caps() {
    int i, j;
    const char *term = getenv("TERM");

    if (!term) {
        return TB_ERR_NO_TERM;
    }

    // Check for exact TERM match
    for (i = 0; builtin_terms[i].name != NULL; i++) {
        if (strcmp(term, builtin_terms[i].name) == 0) {
            for (j = 0; j < TB_CAP__COUNT; j++) {
                global.caps[j] = builtin_terms[i].caps[j];
            }
            return TB_OK;
        }
    }

    // Check for partial TERM or alias match
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

static const char *get_terminfo_string(int16_t str_offsets_pos,
    int16_t str_table_pos, int16_t str_table_len, int16_t str_index) {
    const int16_t *str_offset =
        (int16_t *)(global.terminfo + (int)str_offsets_pos +
                    ((int)str_index * (int)sizeof(int16_t)));
    if (*str_offset >= str_table_len) {
        // Invalid string offset
        return NULL;
    }
    if (((size_t)((int)str_table_pos + (int)*str_offset)) >= global.nterminfo) {
        // Truncated/corrupt terminfo?
        return NULL;
    }
    return (
        const char *)(global.terminfo + (int)str_table_pos + (int)*str_offset);
}

static int wait_event(struct tb_event *event, int timeout) {
    int rv;
    char buf[TB_OPT_READ_BUF];

    memset(event, 0, sizeof(*event));
    if_ok_return(rv, extract_event(event));

#if !TB_OPT_SELECT
    struct pollfd fds[2] = {
        {.fd = global.rfd,              .events = POLLIN},
        {.fd = global.resize_pipefd[0], .events = POLLIN}
    };
#else
    fd_set fds;
    struct timeval tv;
    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout - (tv.tv_sec * 1000)) * 1000;
#endif

    do {
#if !TB_OPT_SELECT
        int poll_rv = poll(fds, 2, timeout);
#else
        FD_ZERO(&fds);
        FD_SET(global.rfd, &fds);
        FD_SET(global.resize_pipefd[0], &fds);

        int maxfd = global.resize_pipefd[0] > global.rfd
                        ? global.resize_pipefd[0]
                        : global.rfd;

        int poll_rv =
            select(maxfd + 1, &fds, NULL, NULL, (timeout < 0) ? NULL : &tv);
#endif

        if (poll_rv < 0) {
            // Let EINTR/EAGAIN bubble up
            global.last_errno = errno;
            return TB_ERR_POLL;
        } else if (poll_rv == 0) {
            return TB_ERR_NO_EVENT;
        }

#if !TB_OPT_SELECT
        int tty_has_events = (fds[0].revents & POLLIN);
        int resize_has_events = (fds[1].revents & POLLIN);
#else
        int tty_has_events = (FD_ISSET(global.rfd, &fds));
        int resize_has_events = (FD_ISSET(global.resize_pipefd[0], &fds));
#endif

        if (tty_has_events) {
            ssize_t read_rv = read(global.rfd, buf, sizeof(buf));
            if (read_rv < 0) {
                global.last_errno = errno;
                return TB_ERR_READ;
            } else if (read_rv > 0) {
                bytebuf_nputs(&global.in, buf, read_rv);
            }
        }

        if (resize_has_events) {
            int ignore = 0;
            read(global.resize_pipefd[0], &ignore, sizeof(ignore));
            if_err_return(rv, update_term_size());
            event->type = TB_EVENT_RESIZE;
            event->w = global.width;
            event->h = global.height;
            global.need_resize = 1;
            return TB_OK;
        }

        memset(event, 0, sizeof(*event));
        if_ok_return(rv, extract_event(event));
    } while (timeout == -1);

    return rv;
}

static int extract_event(struct tb_event *event) {
    int rv;
    struct bytebuf_t *in = &global.in;

    if (in->len == 0) {
        return TB_ERR;
    }

    if (in->buf[0] == '\x1b') {
        // Escape sequence?
        // In TB_INPUT_ESC, skip if the buffer is a single escape char
        if (!((global.input_mode & TB_INPUT_ESC) && in->len == 1)) {
            if_ok_or_need_more_return(rv, extract_esc(event));
        }

        // Escape key?
        if (global.input_mode & TB_INPUT_ESC) {
            event->type = TB_EVENT_KEY;
            event->ch = 0;
            event->key = TB_KEY_ESC;
            event->mod = 0;
            bytebuf_shift(in, 1);
            return TB_OK;
        }

        // Recurse for alt key
        event->mod |= TB_MOD_ALT;
        bytebuf_shift(in, 1);
        return extract_event(event);
    }

    // ASCII control key?
    if ((uint16_t)in->buf[0] < TB_KEY_SPACE || in->buf[0] == TB_KEY_BACKSPACE2)
    {
        event->type = TB_EVENT_KEY;
        event->ch = 0;
        event->key = (uint16_t)in->buf[0];
        event->mod |= TB_MOD_CTRL;
        bytebuf_shift(in, 1);
        return TB_OK;
    }

    // UTF-8?
    if (in->len >= (size_t)tb_utf8_char_length(in->buf[0])) {
        event->type = TB_EVENT_KEY;
        tb_utf8_char_to_unicode(&event->ch, in->buf);
        event->key = 0;
        bytebuf_shift(in, tb_utf8_char_length(in->buf[0]));
        return TB_OK;
    }

    // Need more input
    return TB_ERR;
}

static int extract_esc(struct tb_event *event) {
    int rv;
    if_ok_or_need_more_return(rv, extract_esc_user(event, 0));
    if_ok_or_need_more_return(rv, extract_esc_cap(event));
    if_ok_or_need_more_return(rv, extract_esc_mouse(event));
    if_ok_or_need_more_return(rv, extract_esc_user(event, 1));
    return TB_ERR;
}

static int extract_esc_user(struct tb_event *event, int is_post) {
    int rv;
    size_t consumed = 0;
    struct bytebuf_t *in = &global.in;
    int (*fn)(struct tb_event *, size_t *);

    fn = is_post ? global.fn_extract_esc_post : global.fn_extract_esc_pre;

    if (!fn) {
        return TB_ERR;
    }

    rv = fn(event, &consumed);
    if (rv == TB_OK) {
        bytebuf_shift(in, consumed);
    }

    if_ok_or_need_more_return(rv, rv);
    return TB_ERR;
}

static int extract_esc_cap(struct tb_event *event) {
    int rv;
    struct bytebuf_t *in = &global.in;
    struct cap_trie_t *node;
    size_t depth;

    if_err_return(rv, cap_trie_find(in->buf, in->len, &node, &depth));
    if (node->is_leaf) {
        // Found a leaf node
        event->type = TB_EVENT_KEY;
        event->ch = 0;
        event->key = node->key;
        event->mod = node->mod;
        bytebuf_shift(in, depth);
        return TB_OK;
    } else if (node->nchildren > 0 && in->len <= depth) {
        // Found a branch node (not enough input)
        return TB_ERR_NEED_MORE;
    }

    return TB_ERR;
}

static int extract_esc_mouse(struct tb_event *event) {
    struct bytebuf_t *in = &global.in;

    enum type { TYPE_VT200 = 0, TYPE_1006, TYPE_1015, TYPE_MAX };

    char *cmp[TYPE_MAX] = {//
        // X10 mouse encoding, the simplest one
        // \x1b [ M Cb Cx Cy
        [TYPE_VT200] = "\x1b[M",
        // xterm 1006 extended mode or urxvt 1015 extended mode
        // xterm: \x1b [ < Cb ; Cx ; Cy (M or m)
        [TYPE_1006] = "\x1b[<",
        // urxvt: \x1b [ Cb ; Cx ; Cy M
        [TYPE_1015] = "\x1b["};

    enum type type = 0;
    int ret = TB_ERR;

    // Unrolled at compile-time (probably)
    for (; type < TYPE_MAX; type++) {
        size_t size = strlen(cmp[type]);

        if (in->len >= size && (strncmp(cmp[type], in->buf, size)) == 0) {
            break;
        }
    }

    if (type == TYPE_MAX) {
        ret = TB_ERR; // No match
        return ret;
    }

    size_t buf_shift = 0;

    switch (type) {
        case TYPE_VT200:
            if (in->len >= 6) {
                int b = in->buf[3] - 0x20;
                int fail = 0;

                switch (b & 3) {
                    case 0:
                        event->key = ((b & 64) != 0) ? TB_KEY_MOUSE_WHEEL_UP
                                                     : TB_KEY_MOUSE_LEFT;
                        break;
                    case 1:
                        event->key = ((b & 64) != 0) ? TB_KEY_MOUSE_WHEEL_DOWN
                                                     : TB_KEY_MOUSE_MIDDLE;
                        break;
                    case 2:
                        event->key = TB_KEY_MOUSE_RIGHT;
                        break;
                    case 3:
                        event->key = TB_KEY_MOUSE_RELEASE;
                        break;
                    default:
                        ret = TB_ERR;
                        fail = 1;
                        break;
                }

                if (!fail) {
                    if ((b & 32) != 0) {
                        event->mod |= TB_MOD_MOTION;
                    }

                    // the coord is 1,1 for upper left
                    event->x = ((uint8_t)in->buf[4]) - 0x21;
                    event->y = ((uint8_t)in->buf[5]) - 0x21;

                    ret = TB_OK;
                }

                buf_shift = 6;
            }
            break;
        case TYPE_1006:
            // fallthrough
        case TYPE_1015: {
            size_t index_fail = (size_t)-1;

            enum {
                FIRST_M = 0,
                FIRST_SEMICOLON,
                LAST_SEMICOLON,
                FIRST_LAST_MAX
            };

            size_t indices[FIRST_LAST_MAX] = {index_fail, index_fail,
                index_fail};
            int m_is_capital = 0;

            for (size_t i = 0; i < in->len; i++) {
                if (in->buf[i] == ';') {
                    if (indices[FIRST_SEMICOLON] == index_fail) {
                        indices[FIRST_SEMICOLON] = i;
                    } else {
                        indices[LAST_SEMICOLON] = i;
                    }
                } else if (indices[FIRST_M] == index_fail) {
                    if (in->buf[i] == 'm' || in->buf[i] == 'M') {
                        m_is_capital = (in->buf[i] == 'M');
                        indices[FIRST_M] = i;
                    }
                }
            }

            if (indices[FIRST_M] == index_fail ||
                indices[FIRST_SEMICOLON] == index_fail ||
                indices[LAST_SEMICOLON] == index_fail)
            {
                ret = TB_ERR;
            } else {
                int start = (type == TYPE_1015 ? 2 : 3);

                int n1 = strtoul(&in->buf[start], NULL, 10);
                int n2 =
                    strtoul(&in->buf[indices[FIRST_SEMICOLON] + 1], NULL, 10);
                int n3 =
                    strtoul(&in->buf[indices[LAST_SEMICOLON] + 1], NULL, 10);

                if (type == TYPE_1015) {
                    n1 -= 0x20;
                }

                int fail = 0;

                switch (n1 & 3) {
                    case 0:
                        event->key = ((n1 & 64) != 0) ? TB_KEY_MOUSE_WHEEL_UP
                                                      : TB_KEY_MOUSE_LEFT;
                        break;
                    case 1:
                        event->key = ((n1 & 64) != 0) ? TB_KEY_MOUSE_WHEEL_DOWN
                                                      : TB_KEY_MOUSE_MIDDLE;
                        break;
                    case 2:
                        event->key = TB_KEY_MOUSE_RIGHT;
                        break;
                    case 3:
                        event->key = TB_KEY_MOUSE_RELEASE;
                        break;
                    default:
                        ret = TB_ERR;
                        fail = 1;
                        break;
                }

                buf_shift = in->len;

                if (!fail) {
                    if (!m_is_capital) {
                        // on xterm mouse release is signaled by lowercase m
                        event->key = TB_KEY_MOUSE_RELEASE;
                    }

                    if ((n1 & 32) != 0) {
                        event->mod |= TB_MOD_MOTION;
                    }

                    event->x = ((uint8_t)n2) - 1;
                    event->y = ((uint8_t)n3) - 1;

                    ret = TB_OK;
                }
            }
        } break;
        case TYPE_MAX:
            ret = TB_ERR;
    }

    if (buf_shift > 0) {
        bytebuf_shift(in, buf_shift);
    }

    if (ret == TB_OK) {
        event->type = TB_EVENT_MOUSE;
    }

    return ret;
}

static int resize_if_needed() {
    int rv;
    if (!global.need_resize) {
        return TB_OK;
    }
    if_err_return(rv, update_term_size()); // TODO is this needed?
    if_err_return(rv,
        cellbuf_resize(&global.back, global.width, global.height));
    if_err_return(rv,
        cellbuf_resize(&global.front, global.width, global.height));
    if_err_return(rv, cellbuf_clear(&global.front));
    if_err_return(rv, send_clear());
    global.need_resize = 0;
    return TB_OK;
}

static void handle_resize(int sig) {
    int errno_copy = errno;
    write(global.resize_pipefd[1], &sig, sizeof(sig));
    errno = errno_copy;
}

static int send_attr(uintattr_t fg, uintattr_t bg) {
    int rv;

    if (fg == global.last_fg && bg == global.last_bg) {
        return TB_OK;
    }

    if_err_return(rv, bytebuf_puts(&global.out, global.caps[TB_CAP_SGR0]));

    uintattr_t cfg, cbg;
    switch (global.output_mode) {
        default:
        case TB_OUTPUT_NORMAL:
            cfg = fg & 0x0f;
            cbg = bg & 0x0f;
            break;

        case TB_OUTPUT_256:
            cfg = fg & 0xff;
            cbg = bg & 0xff;
            break;

        case TB_OUTPUT_216:
            cfg = fg & 0xff;
            if (cfg > 215)
                cfg = 7;
            cbg = bg & 0xff;
            if (cbg > 215)
                cbg = 0;
            cfg += 0x10;
            cbg += 0x10;
            break;

        case TB_OUTPUT_GRAYSCALE:
            cfg = fg & 0xff;
            if (cfg > 23)
                cfg = 23;
            cbg = bg & 0xff;
            if (cbg > 23)
                cbg = 0;
            cfg += 0xe8;
            cbg += 0xe8;
            break;

        case TB_OUTPUT_TRUECOLOR:
            cfg = fg;
            cbg = bg;
            break;
    }

    if (global.output_mode != TB_OUTPUT_TRUECOLOR) {
        if (fg & TB_BOLD)
            if_err_return(rv,
                bytebuf_puts(&global.out, global.caps[TB_CAP_BOLD]));

        if (bg & TB_BOLD)
            if_err_return(rv,
                bytebuf_puts(&global.out, global.caps[TB_CAP_BLINK]));

        if (fg & TB_UNDERLINE)
            if_err_return(rv,
                bytebuf_puts(&global.out, global.caps[TB_CAP_UNDERLINE]));

        if (fg & TB_ITALIC)
            if_err_return(rv,
                bytebuf_puts(&global.out, global.caps[TB_CAP_ITALIC]));

        if ((fg & TB_REVERSE) || (bg & TB_REVERSE))
            if_err_return(rv,
                bytebuf_puts(&global.out, global.caps[TB_CAP_REVERSE]));
    }

    if_err_return(rv, send_sgr(cfg, cbg));

    global.last_fg = fg;
    global.last_bg = bg;

    return TB_OK;
}

static int send_sgr(uintattr_t fg, uintattr_t bg) {
    int rv;
    char nbuf[32];

    if (global.output_mode != TB_OUTPUT_TRUECOLOR && fg == TB_DEFAULT &&
        bg == TB_DEFAULT)
    {
        return TB_OK;
    }

    switch (global.output_mode) {
        default:
        case TB_OUTPUT_NORMAL:
            send_literal(rv, "\x1b[");
            if (fg != TB_DEFAULT) {
                send_literal(rv, "3");
                send_num(rv, nbuf, fg - 1);
                if (bg != TB_DEFAULT) {
                    send_literal(rv, ";");
                }
            }
            if (bg != TB_DEFAULT) {
                send_literal(rv, "4");
                send_num(rv, nbuf, bg - 1);
            }
            send_literal(rv, "m");
            break;

        case TB_OUTPUT_256:
        case TB_OUTPUT_216:
        case TB_OUTPUT_GRAYSCALE:
            send_literal(rv, "\x1b[");
            if (fg != TB_DEFAULT) {
                send_literal(rv, "38;5;");
                send_num(rv, nbuf, fg);
                if (bg != TB_DEFAULT) {
                    send_literal(rv, ";");
                }
            }
            if (bg != TB_DEFAULT) {
                send_literal(rv, "48;5;");
                send_num(rv, nbuf, bg);
            }
            send_literal(rv, "m");
            break;

        case TB_OUTPUT_TRUECOLOR:
            send_literal(rv, "\x1b[38;2;");
            send_num(rv, nbuf, (fg >> 16) & 0xff);
            send_literal(rv, ";");
            send_num(rv, nbuf, (fg >> 8) & 0xff);
            send_literal(rv, ";");
            send_num(rv, nbuf, fg & 0xff);
            send_literal(rv, ";48;2;");
            send_num(rv, nbuf, (bg >> 16) & 0xff);
            send_literal(rv, ";");
            send_num(rv, nbuf, (bg >> 8) & 0xff);
            send_literal(rv, ";");
            send_num(rv, nbuf, bg & 0xff);
            send_literal(rv, "m");
            break;
    }
    return TB_OK;
}

static int send_cursor_if(int x, int y) {
    int rv;
    char nbuf[32];
    if (x < 0 || y < 0) {
        return TB_OK;
    }
    send_literal(rv, "\x1b[");
    send_num(rv, nbuf, y + 1);
    send_literal(rv, ";");
    send_num(rv, nbuf, x + 1);
    send_literal(rv, "H");
    return TB_OK;
}

static int send_char(int x, int y, uint32_t ch) {
    return send_cluster(x, y, &ch, 1);
}

static int send_cluster(int x, int y, uint32_t *ch, size_t nch) {
    int rv;
    char abuf[8];

    if (global.last_x != x - 1 || global.last_y != y) {
        if_err_return(rv, send_cursor_if(x, y));
    }
    global.last_x = x;
    global.last_y = y;

    int i;
    for (i = 0; i < (int)nch; i++) {
        uint32_t ach = *(ch + i);
        int aw = tb_utf8_unicode_to_char(abuf, ach);
        if (!ach) {
            abuf[0] = ' ';
        }
        if_err_return(rv, bytebuf_nputs(&global.out, abuf, (size_t)aw));
    }

    return TB_OK;
}

static int convert_num(uint32_t num, char *buf) {
    int i, l = 0;
    int ch;
    do {
        buf[l++] = '0' + (num % 10);
        num /= 10;
    } while (num);
    for (i = 0; i < l / 2; i++) {
        ch = buf[i];
        buf[i] = buf[l - 1 - i];
        buf[l - 1 - i] = ch;
    }
    return l;
}

static int cell_cmp(struct tb_cell *a, struct tb_cell *b) {
    if (a->ch != b->ch || a->fg != b->fg || a->bg != b->bg) {
        return 1;
    }
#ifdef TB_OPT_EGC
    if (a->nech != b->nech) {
        return 1;
    } else if (a->nech > 0) { // a->nech == b->nech
        return memcmp(a->ech, b->ech, a->nech);
    }
#endif
    return 0;
}

static int cell_copy(struct tb_cell *dst, struct tb_cell *src) {
#ifdef TB_OPT_EGC
    if (src->nech > 0) {
        return cell_set(dst, src->ech, src->nech, src->fg, src->bg);
    }
#endif
    return cell_set(dst, &src->ch, 1, src->fg, src->bg);
}

static int cell_set(struct tb_cell *cell, uint32_t *ch, size_t nch,
    uintattr_t fg, uintattr_t bg) {
    cell->ch = ch ? *ch : 0;
    cell->fg = fg;
    cell->bg = bg;
#ifdef TB_OPT_EGC
    if (nch <= 1) {
        cell->nech = 0;
    } else {
        cell_reserve_ech(cell, nch + 1);
        memcpy(cell->ech, ch, nch);
        cell->ech[nch] = '\0';
        cell->nech = nch;
    }
#else
    (void)nch;
    (void)cell_reserve_ech;
#endif
    return TB_OK;
}

static int cell_reserve_ech(struct tb_cell *cell, size_t n) {
#ifdef TB_OPT_EGC
    if (cell->cech >= n) {
        return TB_OK;
    }
    if (!(cell->ech = tb_realloc(cell->ech, n * sizeof(cell->ch)))) {
        return TB_ERR_MEM;
    }
    cell->cech = n;
    return TB_OK;
#else
    (void)cell;
    (void)n;
    return TB_ERR;
#endif
}

static int cell_free(struct tb_cell *cell) {
#ifdef TB_OPT_EGC
    if (cell->ech) {
        tb_free(cell->ech);
    }
#endif
    memset(cell, 0, sizeof(*cell));
    return TB_OK;
}

static int cellbuf_init(struct cellbuf_t *c, int w, int h) {
    c->cells = tb_malloc(sizeof(struct tb_cell) * w * h);
    if (!c->cells) {
        return TB_ERR_MEM;
    }
    memset(c->cells, 0, sizeof(struct tb_cell) * w * h);
    c->width = w;
    c->height = h;
    return TB_OK;
}

static int cellbuf_free(struct cellbuf_t *c) {
    if (c->cells) {
        int i;
        for (i = 0; i < c->width * c->height; i++) {
            cell_free(&c->cells[i]);
        }
        tb_free(c->cells);
    }
    memset(c, 0, sizeof(*c));
    return TB_OK;
}

static int cellbuf_clear(struct cellbuf_t *c) {
    int i;
    uint32_t space = (uint32_t)' ';
    for (i = 0; i < c->width * c->height; i++) {
        cell_set(&c->cells[i], &space, 1, global.fg, global.bg);
    }
    return TB_OK;
}

static int cellbuf_get(struct cellbuf_t *c, int x, int y,
    struct tb_cell **out) {
    if (x < 0 || x >= c->width || y < 0 || y >= c->height) {
        *out = NULL;
        return TB_ERR_OUT_OF_BOUNDS;
    }
    *out = &c->cells[(y * c->width) + x];
    return TB_OK;
}

static int cellbuf_resize(struct cellbuf_t *c, int w, int h) {
    int rv;

    int ow = c->width;
    int oh = c->height;

    if (ow == w && oh == h) {
        return TB_OK;
    }

    w = w < 1 ? 1 : w;
    h = h < 1 ? 1 : h;

    int minw = (w < ow) ? w : ow;
    int minh = (h < oh) ? h : oh;

    struct tb_cell *prev = c->cells;

    if_err_return(rv, cellbuf_init(c, w, h));
    if_err_return(rv, cellbuf_clear(c));

    int x, y;
    for (x = 0; x < minw; x++) {
        for (y = 0; y < minh; y++) {
            struct tb_cell *src, *dst;
            src = &prev[(y * ow) + x];
            if_err_return(rv, cellbuf_get(c, x, y, &dst));
            if_err_return(rv, cell_copy(dst, src));
        }
    }

    tb_free(prev);

    return TB_OK;
}

static int bytebuf_puts(struct bytebuf_t *b, const char *str) {
    return bytebuf_nputs(b, str, (size_t)strlen(str));
}

static int bytebuf_nputs(struct bytebuf_t *b, const char *str, size_t nstr) {
    int rv;
    if_err_return(rv, bytebuf_reserve(b, b->len + nstr + 1));
    memcpy(b->buf + b->len, str, nstr);
    b->len += nstr;
    b->buf[b->len] = '\0';
    return TB_OK;
}

static int bytebuf_shift(struct bytebuf_t *b, size_t n) {
    if (n > b->len) {
        n = b->len;
    }
    size_t nmove = b->len - n;
    memmove(b->buf, b->buf + n, nmove);
    b->len -= n;
    return TB_OK;
}

static int bytebuf_flush(struct bytebuf_t *b, int fd) {
    if (b->len <= 0) {
        return TB_OK;
    }
    ssize_t write_rv = write(fd, b->buf, b->len);
    if (write_rv < 0 || (size_t)write_rv != b->len) {
        // Note, errno will be 0 on partial write
        global.last_errno = errno;
        return TB_ERR;
    }
    b->len = 0;
    return TB_OK;
}

static int bytebuf_reserve(struct bytebuf_t *b, size_t sz) {
    if (b->cap >= sz) {
        return TB_OK;
    }
    size_t newcap = b->cap > 0 ? b->cap : 1;
    while (newcap < sz) {
        newcap *= 2;
    }
    char *newbuf;
    if (b->buf) {
        newbuf = tb_realloc(b->buf, newcap);
    } else {
        newbuf = tb_malloc(newcap);
    }
    if (!newbuf) {
        return TB_ERR_MEM;
    }
    b->buf = newbuf;
    b->cap = newcap;
    return TB_OK;
}

static int bytebuf_free(struct bytebuf_t *b) {
    if (b->buf) {
        tb_free(b->buf);
    }
    memset(b, 0, sizeof(*b));
    return TB_OK;
}

#endif /* TB_IMPL */
