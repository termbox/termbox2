// rustc example.rs -L.. -ltermbox2 && LD_LIBRARY_PATH=.. ./example

use std::os::raw::c_uint;

#[repr(C)]
#[derive(Default)]
struct TbEvent {
    type_: u8,
    mod_: u8,
    key: u16,
    ch: u32,
    w: i32,
    h: i32,
    x: i32,
    y: i32,
}

extern "C" {
    fn tb_init() -> i32;
    fn tb_shutdown() -> i32;
    fn tb_printf(x: i32, y: i32, fg: u32, bg: u32, fmt: *const u8, ...) -> i32;
    fn tb_present() -> i32;
    fn tb_poll_event(event: *mut TbEvent) -> i32;
}

fn main() {
    unsafe {
        main_unsafe();
    }
}

unsafe fn main_unsafe() {
    let mut ev = TbEvent::default();

    tb_init();

    let mut y = 0;
    tb_printf(0, y, (0x0002 | 0x0100) as u32, 0, "hello world from rust\0".as_ptr() as *const u8);
    y += 1;
    tb_printf(0, y, 3, 0, "press any key\0".as_ptr() as *const u8);
    y += 1;
    tb_present();
    tb_poll_event(&mut ev);

    tb_printf(0, y, 4, 0,
        "event: type=%d mod=%d key=%d ch=%d w=%d h=%d x=%d y=%d\0".as_ptr() as *const u8,
        ev.type_ as c_uint,
        ev.mod_ as c_uint,
        ev.key as c_uint,
        ev.ch,
        ev.w,
        ev.h,
        ev.x,
        ev.y,
    );
    y += 1;
    tb_present();

    tb_printf(0, y, 5, 0, "press any key to quit\0".as_ptr() as *const u8);
    tb_present();
    tb_poll_event(&mut ev);

    tb_shutdown();
}
