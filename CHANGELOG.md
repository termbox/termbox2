# Changelog

## [2.7.0] - 2026-11-07

### Added

* Support modifier keys in mouse mode ([`e773c92`](https://github.com/termbox/termbox2/commit/e773c92)) (Ingvix)
* Add `tb_get_cell` ([`b54fe6f`](https://github.com/termbox/termbox2/commit/b54fe6f))
* Add built-in replacements for `iswprint(3)` and `wcwidth(3)` ([`a6060a5`](https://github.com/termbox/termbox2/commit/a6060a5))

### Changed

* Null-terminate terminfo to avoid overrun on corrupt entries ([`04623ca`](https://github.com/termbox/termbox2/commit/04623ca))
* Ignore zero width codepoints when `TB_OPT_EGC` is off instead of erroring ([`3f9afbd`](https://github.com/termbox/termbox2/commit/3f9afbd))
* Add fast path to `tb_iswprint_ex` for all 1-byte codepoints ([`cac1901`](https://github.com/termbox/termbox2/commit/cac1901))

### Fixed

* Fix memory leak in `cellbuf_resize` with grapheme clusters ([`f2c15ae`](https://github.com/termbox/termbox2/commit/f2c15ae))
* Fix bug in `cell_cmp` that was breaking grapheme cluster comparisons ([`7dec904`](https://github.com/termbox/termbox2/commit/7dec904))
* Guard `tb_send` and `tb_sendf` with initialization check ([`15df20a`](https://github.com/termbox/termbox2/commit/15df20a))
* Fix fd leak when non-tty is passed to `tb_init_file` ([`38c3d33`](https://github.com/termbox/termbox2/commit/38c3d33))
* Fix calculation of grapheme cluster width ([`918369f`](https://github.com/termbox/termbox2/commit/918369f))
* Tolerate incomplete sequences in `extract_esc_mouse` (`TB_ERR_NEED_MORE`) ([`48e3d85`](https://github.com/termbox/termbox2/commit/48e3d85))
* Fix possible buffer over-read in `get_terminfo_int16` ([`cdf788f`](https://github.com/termbox/termbox2/commit/cdf788f)) (Grigory Kirillov)
* Fix terminfo lookup according to ncurses behavior ([`0b81603`](https://github.com/termbox/termbox2/commit/0b81603)) (Grigory Kirillov)
* Fix mouse coordinate truncation in 1006 and 1015 modes ([`511ff2f`](https://github.com/termbox/termbox2/commit/511ff2f)) (mivirl)
* Fix validation check of terminfo header ([`8ee9dc1`](https://github.com/termbox/termbox2/commit/8ee9dc1)) (Chongyu Guo)
* Fix bug in `tb_iswprint_ex` with `TB_OPT_LIBC_WCHAR` on ([`c95f7b1`](https://github.com/termbox/termbox2/commit/c95f7b1))
* Fix C++ compile errors and warnings ([`19ad32d`](https://github.com/termbox/termbox2/commit/19ad32d), [`83027d4`](https://github.com/termbox/termbox2/commit/83027d4)) (korei999, Oskar Strengbohm)

## [2.5.0] - 2024-12-27

### Added

* Add compile-time option `TB_OPT_ATTR_W` for determining whether to 16-, 32- or 64-bit attributes ([`fcb8dc5`](https://github.com/termbox/termbox2/commit/fcb8dc5))
* Add attributes `TB_BRIGHT`, `TB_DIM`, `TB_INVISIBLE`, `TB_STRIKEOUT`, `TB_UNDERLINE_2`, and `TB_OVERLINE` ([`fcb8dc5`](https://github.com/termbox/termbox2/commit/fcb8dc5))
* Add Rust demo ([`282e292`](https://github.com/termbox/termbox2/commit/282e292)) (rilysh)
* Add `tb_invalidate` ([`755ab64`](https://github.com/termbox/termbox2/commit/755ab64))

### Changed

* Tolerate newlines, non-printable codepoints, and out-of-bounds in `tb_print*` ([`b075ddd`](https://github.com/termbox/termbox2/commit/b075ddd))
* Send non-printable codepoints as U+FFFD ([`cd42ba3`](https://github.com/termbox/termbox2/commit/cd42ba3))
* Do not use `__` prefix with include guard ([`1974ba8`](https://github.com/termbox/termbox2/commit/1974ba8)) (iusevoidbtw)
* Add `const` qualifier to mouse escape sequence constants ([`f150ad3`](https://github.com/termbox/termbox2/commit/f150ad3)) (Danylo Kondratiev)
* Null-terminate output of `tb_utf8_unicode_to_char` ([`5861caf`](https://github.com/termbox/termbox2/commit/5861caf))
* Stop reading at null-terminator in `tb_utf8_char_to_unicode` ([`ce6488c`](https://github.com/termbox/termbox2/commit/ce6488c))
* Remove extraneous `if_not_init_return` check in `tb_set_cell` ([`a8e2644`](https://github.com/termbox/termbox2/commit/a8e2644)) (Mitchell Rosen)
* Parse test macros from `gcc -E -dM` output instead of the raw header file ([`9f018e5`](https://github.com/termbox/termbox2/commit/9f018e5))

### Fixed

* Fix memory errors in grapheme cluster handling ([`b89b08c`](https://github.com/termbox/termbox2/commit/b89b08c), [`25c3f8b`](https://github.com/termbox/termbox2/commit/25c3f8b))
* Fix rendering of combining characters with double-width chars ([`7487f1b`](https://github.com/termbox/termbox2/commit/7487f1b))
* Fix invalidation of cells that were skipped due to double-width chars ([`aaf10e7`](https://github.com/termbox/termbox2/commit/aaf10e7))
* Fix incorrect copying of grapheme clusters ([`f5d6386`](https://github.com/termbox/termbox2/commit/f5d6386))
* Fix compile warnings ([`3f11ca0`](https://github.com/termbox/termbox2/commit/3f11ca0), [`f2ee841`](https://github.com/termbox/termbox2/commit/f2ee841)) (Daniel)

### Deprecated

* Deprecate `TB_256_BLACK` and `TB_TRUECOLOR_BLACK` in favor of `TB_HI_BLACK` ([`fcb8dc5`](https://github.com/termbox/termbox2/commit/fcb8dc5))
* Deprecate `TB_OPT_TRUECOLOR` in favor of `TB_OPT_ATTR_W` ([`fcb8dc5`](https://github.com/termbox/termbox2/commit/fcb8dc5))
* Deprecate `TB_TRUECOLOR_*` in favor of `TB_*` equivalents ([`fcb8dc5`](https://github.com/termbox/termbox2/commit/fcb8dc5))

## [2.0.0] - 2022-09-18

- Initial release

[2.7.0]: https://github.com/termbox/termbox2/releases/tag/v2.7.0
[2.5.0]: https://github.com/termbox/termbox2/releases/tag/v2.5.0
[2.0.0]: https://github.com/termbox/termbox2/releases/tag/v2.0.0
