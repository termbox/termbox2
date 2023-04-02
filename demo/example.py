#!/usr/bin/env python3
import cffi # requires python3-cffi

ffi = cffi.FFI()

ffi.cdef("""
    struct tb_event {
        uint8_t type;
        uint8_t mod;
        uint16_t key;
        uint32_t ch;
        int32_t w;
        int32_t h;
        int32_t x;
        int32_t y;
    };
    int tb_init();
    int tb_shutdown();
    int tb_present();
    int tb_poll_event(struct tb_event *event);
    int tb_printf(int x, int y, uint32_t fg, uint32_t bg, const char *fmt, ...);
""")
termbox = ffi.dlopen("../libtermbox2.so")
ev = ffi.new("struct tb_event *")

termbox.tb_init()

y = 0
termbox.tb_printf(0, y, 0x02 | 0x0100, 0x00, b"hello from python"); y += 1
termbox.tb_printf(0, y, 0x03, 0x00, b"press any key"); y += 1
termbox.tb_present()
termbox.tb_poll_event(ev);

termbox.tb_printf(0, y, 0x04, 0x00,
    b"event: type=%d mod=%d key=%d ch=%d w=%d h=%d x=%d y=%d",
    ffi.cast("int", ev.type),
    ffi.cast("int", ev.mod),
    ffi.cast("int", ev.key),
    ffi.cast("int", ev.ch),
    ffi.cast("int", ev.w),
    ffi.cast("int", ev.h),
    ffi.cast("int", ev.x),
    ffi.cast("int", ev.y)); y += 1
termbox.tb_present()

termbox.tb_printf(0, y, 0x05, 0x00, b"press any key to quit"); y += 1
termbox.tb_present()
termbox.tb_poll_event(ev);

termbox.tb_shutdown()
