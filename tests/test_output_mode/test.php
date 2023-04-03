<?php
declare(strict_types=1);

$test->ffi->tb_init();

$y = 0;
$fg = 7;

$test->ffi->tb_printf(0, $y++, $fg, 0, "cyan (even after mode switch)");
$test->ffi->tb_present();

$test->ffi->tb_set_output_mode($test->defines['TB_OUTPUT_GRAYSCALE']);

$test->ffi->tb_printf(0, $y++, $fg, 0, "gray");
$test->ffi->tb_present();

$test->screencap();
