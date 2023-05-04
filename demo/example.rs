// Create a new project with cargo new [project_name] and generate Rust bindings
// for termbox2. Place the binding file in the newly created project's src directly
// and create a build script in the project directory followed below.

// println!("cargo:rustc-flags= -L./lib/ -ltermbox2");
// This specifies the Rust compiler the location of the termbox2 shared library
// and link with it. For a demo project, see: https://github.com/rilysh/termbox2-ww

mod termbox2;

use std::ffi::c_uint;

fn main() {
    unsafe {
        termbox2::tb_init();

        let mut ev = termbox2::tb_event {
            type_: 0,
            mod_: 0,
            key: 0,
            ch: 0,
            w: 0,
            h: 0,
            x: 0,
            y: 0,
        };

        let mut y = 0;

        termbox2::tb_printf(
          0, y, (termbox2::TB_RED | termbox2::TB_BOLD) as u16, 0, "Hello world from Rust\0".as_ptr() as *const i8,
        );

        y += 1;
        termbox2::tb_printf(
          0, y, 3, 0, "Press any key\0".as_ptr() as *const i8
        );

        y += 1;

        termbox2::tb_present();
        termbox2::tb_poll_event(&mut ev);
        termbox2::tb_printf(
          0, y, 4, 0,
          "Event: type=%d mod=%d key=%d ch=%d w=%d h=%d x=%d y=%d\0".as_ptr() as *const i8,
          ev.type_ as c_uint, ev.mod_ as c_uint, ev.key as c_uint,
          ev.ch as c_uint, ev.w as c_uint, ev.h as c_uint,
          ev.x as c_uint, ev.y as c_uint
        );
        termbox2::tb_present();

        y += 1;
        termbox2::tb_printf(0, y, 5, 0, "Press any key to exit\0".as_ptr() as *const i8);
        termbox2::tb_present();
        termbox2::tb_poll_event(&mut ev);
        termbox2::tb_shutdown();
    }
}
