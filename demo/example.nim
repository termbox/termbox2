# nim c -r example.nim
const libtermbox2 = "../libtermbox2.so"

type Event = object
  etype {.importc :"type".}: uint8
  emod {.importc :"mod".}: uint8
  key: uint16
  ch: uint32
  w, h, x, y: int32

proc tb_init(): cint {.importc, dynlib: libtermbox2.}
proc tb_shutdown(): cint {.importc, dynlib: libtermbox2.}
proc tb_present(): cint {.importc, dynlib: libtermbox2.}
proc tb_poll_event(ev: ptr Event): cint {.importc, dynlib: libtermbox2.}
proc tb_printf(x: cint, y: cint, fg: uint32, bg: uint32, fmt: cstring): cint {.importc, dynlib: libtermbox2, varargs.}

var ev: Event

discard tb_init()

var y: int = 0;
discard tb_printf(0.cint, y.cint, 0x0102.uint32, 0x00.uint32, "hello from nim")
y += 1
discard tb_printf(0.cint, y.cint, 0x03.uint32, 0x00.uint32, "press any key")
y += 1
discard tb_present()

discard tb_poll_event(ev.addr)

discard tb_printf(0.cint, y.cint, 0x04.uint32, 0x00.uint32,
    "event: type=%d mod=%d key=%d ch=%d w=%d h=%d x=%d y=%d",
    ev.etype,
    ev.emod,
    ev.key,
    ev.ch,
    ev.w,
    ev.h,
    ev.x,
    ev.y)
y += 1
discard tb_present()

discard tb_printf(0.cint, y.cint, 0x05.uint32, 0x00.uint32, "press any key to quit")
y += 1
discard tb_present()
discard tb_poll_event(ev.addr)

discard tb_shutdown()
