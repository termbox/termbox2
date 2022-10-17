# termbox2

termbox2 is a terminal rendering library that retains the [suckless][0] spirit
of the original [termbox][1] (simple API, no dependencies beyond libc) and adds
some improvements (strict error checking, more efficient escape sequence
parsing, code gen for built-in escape sequences, opt-in support for 32-bit
color, extended grapheme clusters, test suite). termbox2 is organized as a
single file header library, though it is possible to compile it as a
stand-alone shared or static library.

![keyboard demo](demo/keyboard.gif)

### Synopsis

```c
#define TB_IMPL
#include "termbox.h"
#include <stdio.h>

int main(int argc, char **argv) {
    struct tb_event ev;
    int y = 0;

    tb_init();

    tb_printf(0, y++, TB_GREEN, 0, "hello from termbox");
    tb_printf(0, y++, 0, 0, "width=%d height=%d", tb_width(), tb_height());
    tb_printf(0, y++, 0, 0, "press any key...");
    tb_present();

    tb_poll_event(&ev);

    y++;
    tb_printf(0, y++, 0, 0, "event type=%d key=%d ch=%c", ev.type, ev.key, ev.ch);
    tb_printf(0, y++, 0, 0, "press any key to quit...");
    tb_present();

    tb_poll_event(&ev);
    tb_shutdown();

    return 0;
}
```

### API

The basic API should be pretty self-explanatory. Consult the header file itself
for the complete API and documentation.

```c
int tb_init();
int tb_shutdown();

int tb_width();
int tb_height();

int tb_clear();
int tb_present();

int tb_set_cursor(int cx, int cy);
int tb_hide_cursor();

int tb_set_cell(int x, int y, uint32_t ch, uintattr_t fg, uintattr_t bg);

int tb_peek_event(struct tb_event *event, int timeout_ms);
int tb_poll_event(struct tb_event *event);

int tb_print(int x, int y, uintattr_t fg, uintattr_t bg, const char *str);
int tb_printf(int x, int y, uintattr_t fg, uintattr_t bg, const char *fmt, ...);
```

### How to use termbox2

As mentioned above, there are two options:

1. Copy (or `git submodule`) `termbox.h` into your C project. As normal, include
   the header file wherever you want to use `tb_*` functions, but also be sure
   to `#define TB_IMPL` in exactly one of your source files. (This is a common
   pattern for single file header libraries.)
2. Build termbox2 as a library (either `make libtermbox.so` or
   `make libtermbox.a`) and link as normal.

### Language bindings (FFI and ABI compats)

Basic examples in PHP, Python, Ruby, Go, and Zig are in the `demo/` directory.
(Feel free to submit PRs for other languages.)

### Examples

* [mle][2] - flexible terminal-based text editor
* [ictree][3] - like tree but interactive
* [lavat][4] - lava lamp for the terminal

[0]: https://suckless.org
[1]: https://github.com/termbox/termbox
[2]: https://github.com/adsr/mle
[3]: https://github.com/NikitaIvanovV/ictree
[4]: https://github.com/AngelJumbo/lavat
