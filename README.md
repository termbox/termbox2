# termbox2

termbox2 is a terminal rendering library that retains the [suckless][0] spirit
of the original [termbox][1] (simple API, no dependencies beyond libc) and adds
some improvements (strict error checking, more efficient escape sequence
parsing, code gen for built-in escape sequences, opt-in support for 32-bit
color, extended grapheme clusters, test suite). termbox2 is organized as a
single file header library, though it is possible to compile it as a
stand-alone shared or static library.

[0]: https://suckless.org
[1]: https://github.com/termbox/termbox
