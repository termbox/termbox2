// cc -DTB_IMPL -I.. region.c -o region
#include <termbox2.h>

int main(int argc, char **argv) {
    int sel, y, h, done;

    tb_init_ex(1, TB_INIT_REGION, argc >= 2 ? atoi(argv[1]) : 10);

    done = 0;
    sel = 0;
    while (!done) {
        tb_clear();
        for (y = 0; y < tb_height(); y++) {
            tb_printf(0, y, 0, y == sel ? TB_REVERSE : 0, "Item %d", y);
        }
        tb_present();

        struct tb_event ev;
        tb_poll_event(&ev);
        h = tb_height();

        switch (ev.key) {
            case TB_KEY_ARROW_UP:   --sel; if (sel <  0) sel = 0;      break;
            case TB_KEY_ARROW_DOWN: ++sel; if (sel >= h) sel = h - 1;  break;
            case TB_KEY_ENTER:      done = 1;                          break;
            case TB_KEY_PGUP:       tb_region(h + 1);                  break;
            case TB_KEY_PGDN:       tb_region(h - 1);                  break;
        }
    }
    tb_shutdown();

    printf("You picked item %d\n", sel);
    return 0;
}
