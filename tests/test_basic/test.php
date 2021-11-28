<?php
declare(strict_types=1);

$test->ffi->tb_init();

$w = $test->ffi->tb_width();
$h = $test->ffi->tb_height();

$bg = $test->defines['TB_BLACK'];
$red = $test->defines['TB_RED'];
$green = $test->defines['TB_GREEN'];
$blue = $test->defines['TB_BLUE'];

$test->ffi->tb_printf(0, 0, $red, $bg, "width=%d", $w);
$test->ffi->tb_printf(0, 1, $green, $bg, "height=%d", $h);

$test->xvkbd('\Ca'); // Ctrl-A

$event = $test->ffi->new('struct tb_event');
$rv = $test->ffi->tb_peek_event(FFI::addr($event), 1000);

$out_w = $test->ffi->new('size_t');
$test->ffi->tb_printf_ex(0, 2, $blue, $bg, FFI::addr($out_w), "event rv=%d type=%d mod=%d key=%d ch=%d w=%d h=%d x=%d y=%d",
    $rv,
    $event->type,
    $event->mod,
    $event->key,
    $event->ch,
    $event->w,
    $event->h,
    $event->x,
    $event->y
);

$test->ffi->tb_printf(0, 3, 0, 0, "out_w=%d", $out_w->cdata);

$test->ffi->tb_present();

$test->screencap();
