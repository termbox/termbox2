// go run example.go

package main

/*
#cgo LDFLAGS: -L.. -Wl,-rpath,.. -ltermbox2
#include <stdint.h>
typedef struct tb_event_s {
    uint8_t _type;
    uint8_t mod;
    uint16_t key;
    uint32_t ch;
    int32_t w;
    int32_t h;
    int32_t x;
    int32_t y;
} tb_event;
int tb_init();
int tb_shutdown();
int tb_present();
int tb_poll_event(tb_event *event);
int tb_print(int x, int y, uint32_t fg, uint32_t bg, const char *str);
*/
import "C"
import "fmt"

func main() {
    ev := C.tb_event{}

    C.tb_init()

    y := 0
    C.tb_print(C.int(0), C.int(y), C.uint(0x02 | 0x0100), C.uint(0x00), C.CString("hello from go"))
    y += 1
    C.tb_print(C.int(0), C.int(y), C.uint(0x03), C.uint(0x00), C.CString("press any key"))
    y += 1
    C.tb_present()
    C.tb_poll_event(&ev)

    C.tb_print(C.int(0), C.int(y), C.uint(0x04), C.uint(0x00), C.CString(fmt.Sprintf(
        "event: type=%d mod=%d key=%d ch=%d w=%d h=%d x=%d y=%d",
        ev._type,
        ev.mod,
        ev.key,
        ev.ch,
        ev.w,
        ev.h,
        ev.x,
        ev.y)))
    y += 1
    C.tb_present()

    C.tb_print(C.int(0), C.int(y), C.uint(0x05), C.uint(0x00), C.CString("press any key to quit"))
    y += 1
    C.tb_present()
    C.tb_poll_event(&ev)

    C.tb_shutdown()
}
