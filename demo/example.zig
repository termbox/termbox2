// zig run -I.. -I/usr/include -I/usr/include/x86_64-linux-gnu/ -rpath .. -L.. -ltermbox2 -lc example.zig
// (Is there a way to avoid the explicit system library paths?)

const c = @cImport(@cInclude("termbox2.h"));
const std = @import("std");

pub fn main() void {
    _ = c.tb_init();
    var ev = c.tb_event {
        .type = 0,
        .mod = 0,
        .key = 0,
        .ch = 0,
        .w = 0,
        .h = 0,
        .x = 0,
        .y = 0,
    };

    var y: i32 = 0;
    _ = c.tb_printf(0, y, 0x02 | 0x0100, 0x00, "hello from zig");
    y += 1;
    _ = c.tb_printf(0, y, 0x03, 0x00, "press any key");
    y += 1;
    _ = c.tb_present();
    _ = c.tb_poll_event(&ev);

    _ = c.tb_printf(0, y, 0x04, 0x00,
        "event: type=%d mod=%d key=%d ch=%d w=%d h=%d x=%d y=%d",
        ev.type,
        ev.mod,
        ev.key,
        ev.ch,
        ev.w,
        ev.h,
        ev.x,
        ev.y);
    y += 1;
    _ = c.tb_present();

    _ = c.tb_printf(0, y, 0x05, 0x00, "press any key to quit");
    y += 1;
    _ = c.tb_present();
    _ = c.tb_poll_event(&ev);

    _ = c.tb_shutdown();
}
