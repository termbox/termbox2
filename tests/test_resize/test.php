<?php
declare(strict_types=1);

$libc = FFI::cdef(
    'int raise(int signum);'
);

$test->ffi->tb_init();

$w = $test->ffi->tb_width();
$h = $test->ffi->tb_height();

$libc->raise(SIGWINCH);

$event = $test->ffi->new('struct tb_event');
$rv = $test->ffi->tb_peek_event(FFI::addr($event), 1000);

$test->ffi->tb_printf(0, 0, 0, 0, "event rv=%d type=%d ow=%d oh=%d w=%d h=%d",
    $rv,
    $event->type,
    $w,
    $h,
    $event->w,
    $event->h,
);

$test->ffi->tb_present();

$test->screencap();
