#!/usr/bin/env ruby
require 'ffi'

module Termbox
  extend FFI::Library
  ffi_lib '../libtermbox2.so'
  class Event < FFI::Struct
    layout :type, :uint8,
           :mod, :uint8,
           :key, :uint16,
           :ch, :uint32,
           :w, :int32,
           :h, :int32,
           :x, :int32,
           :y, :int32
  end
  attach_function :tb_init, [], :int
  attach_function :tb_shutdown, [], :int
  attach_function :tb_present, [], :int
  attach_function :tb_poll_event, [Event], :int
  attach_function :tb_printf, [:int, :int, :int, :int, :string, :varargs], :int
end

ev = Termbox::Event.new

Termbox::tb_init

y = -1
Termbox::tb_printf(0, y += 1, 0x02 | 0x0100, 0x00, "hello from ruby")
Termbox::tb_printf(0, y += 1, 0x03, 0x00, "press any key")
Termbox::tb_present
Termbox::tb_poll_event(ev)

Termbox::tb_printf(0, y += 1, 0x04, 0x00,
    "event: type=%d mod=%d key=%d ch=%d w=%d h=%d x=%d y=%d",
    :int, ev[:type],
    :int, ev[:mod],
    :int, ev[:key],
    :int, ev[:ch],
    :int, ev[:w],
    :int, ev[:h],
    :int, ev[:x],
    :int, ev[:y])
Termbox::tb_present

Termbox::tb_printf(0, y += 1, 0x05, 0x00, "press any key to quit")
Termbox::tb_present
Termbox::tb_poll_event(ev)

Termbox::tb_shutdown
