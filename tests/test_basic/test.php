<?php
declare(strict_types=1);

$test->ffi->tb_init();

$w = $test->ffi->tb_width();
$h = $test->ffi->tb_height();

$bg = $test->defines['TB_BLACK'];
$red = $test->defines['TB_RED'];
$green = $test->defines['TB_GREEN'];
$blue = $test->defines['TB_BLUE'];

$y = 0;
$version_str = $test->ffi->tb_version();
$has_version = is_string($version_str) && strlen($version_str) > 0;
$test->ffi->tb_printf(0, $y++, 0, 0, "has_version=%s", $has_version ? 'y' : 'n');
$test->ffi->tb_printf(0, $y++, $red, $bg, "width=%d", $w);
$test->ffi->tb_printf(0, $y++, $green, $bg, "height=%d", $h);
foreach (['TB_BOLD', 'TB_UNDERLINE', 'TB_ITALIC', 'TB_REVERSE', 'TB_BRIGHT', 'TB_DIM'] as $attr) {
    $test->ffi->tb_printf(0, $y++, $blue | $test->defines[$attr], $bg, "attr=%s", $attr);
}

$test->xvkbd('\Ca'); // Ctrl-A

$event = $test->ffi->new('struct tb_event');
$rv = $test->ffi->tb_peek_event(FFI::addr($event), 1000);

$out_w = $test->ffi->new('size_t');
$test->ffi->tb_printf_ex(0, $y++, $blue, $bg, FFI::addr($out_w), "event rv=%d type=%d mod=%d key=%d ch=%d w=%d h=%d x=%d y=%d",
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

$test->ffi->tb_printf(0, $y++, 0, 0, "out_w=%d", $out_w->cdata);

$test->ffi->tb_present();

$test->screencap();
