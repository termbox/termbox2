// rdmd ../libtermbox.a example.d

struct tb_event {
    ubyte type;
    ubyte mod;
    ushort key;
    uint ch;
    int w;
    int h;
    int x;
    int y;
};

extern (C) int tb_init();
extern (C) int tb_shutdown();
extern (C) int tb_present();
extern (C) int tb_poll_event(tb_event *event);
extern (C) int tb_printf(int x, int y, uint fg, uint bg, const char *fmt, ...);

void main() {
    tb_event ev;
    int y = 0;

    tb_init();

    tb_printf(0, y++, 0x02 | 0x0100, 0x00, "hello from d");
    tb_printf(0, y++, 0x03, 0x00, "press any key");

    tb_present();
    tb_poll_event(&ev);

    tb_printf(0, y++, 0x04, 0x00,
        "event: type=%d mod=%d key=%d ch=%d w=%d h=%d x=%d y=%d",
        ev.type,
        ev.mod,
        ev.key,
        ev.ch,
        ev.w,
        ev.h,
        ev.x,
        ev.y);
    tb_printf(0, y++, 0x05, 0x00, "press any key to quit");

    tb_present();
    tb_poll_event(&ev);

    tb_shutdown();
}
