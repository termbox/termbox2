<?php
declare(strict_types=1);

$test->ffi->tb_init();

$w = $test->ffi->tb_width();
$h = $test->ffi->tb_height();
$green = $test->defines['TB_GREEN'];
$cellp = $test->ffi->new('struct tb_cell *');
$back = 1;
$front = 0;

$result = [];

$result['set'] = $test->ffi->tb_set_cell(0, 0, ord('a'), $green, 0); // 0 (TB_OK)

$result['invalid_get'] = $test->ffi->tb_get_cell(-1, -1, $back, FFI::addr($cellp)); // -9 (TB_ERR_OUT_OF_BOUNDS)

$result['back_get'] = $test->ffi->tb_get_cell(0, 0, $back, FFI::addr($cellp)); // 0 (TB_OK)
$result['back_ch'] = chr($cellp->ch); // 'a'
$result['back_fg'] = $cellp->fg; // 3 (green)
$result['back_bg'] = $cellp->bg; // 0

$result['front1_get'] = $test->ffi->tb_get_cell(0, 0, $front, FFI::addr($cellp));
$result['front1_ch'] = chr($cellp->ch); // <space> (front buffer empty)
$result['front1_fg'] = $cellp->fg; // 0
$result['front1_bg'] = $cellp->bg; // 0

$result['present'] = $test->ffi->tb_present(); // 0 (TB_OK) (front buffer now populated)

$result['front2_get'] = $test->ffi->tb_get_cell(0, 0, $front, FFI::addr($cellp)); // 0 (TB_OK)
$result['front2_ch'] = chr($cellp->ch); // 'a'
$result['front2_fg'] = $cellp->fg; // 3 (green)
$result['front2_bg'] = $cellp->bg; // 0

$y = 1;
foreach ($result as $k => $v) $test->ffi->tb_printf(0, $y++, 0, 0, '%s=%s', $k, "$v");

$test->ffi->tb_present();

$test->screencap();
