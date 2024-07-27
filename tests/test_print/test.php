<?php
declare(strict_types=1);

$test->ffi->tb_init();

$w = $test->ffi->tb_width();
$h = $test->ffi->tb_height();

$y = 0;
$test->ffi->tb_print(1, $y++, 0, 0, "line1\nline2\nline3");
$y += 2;

$test->ffi->tb_print(0, $y++, 0, 0, "escape=[\x1b]");
$test->ffi->tb_print(0, $y++, 0, 0, "tab=[\t]");

$oob_rv1 = $test->ffi->tb_print($w, $h, 0, 0, "oob1");
$oob_rv2 = $test->ffi->tb_print(-1, -1, 0, 0, "oob2");
$test->ffi->tb_printf(0, $y++, 0, 0, "oob_rv1=%d", $oob_rv1);
$test->ffi->tb_printf(0, $y++, 0, 0, "oob_rv2=%d", $oob_rv2);

$test->ffi->tb_print($w - 5, $h - 5, 0, 0, str_repeat("0123456789\n", 10));

$test->ffi->tb_present();

$test->screencap();
