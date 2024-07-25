<?php
declare(strict_types=1);

$test->ffi->tb_init();

$y = 0;
$test->ffi->tb_print_ex(0, $y++, 0, 0, NULL, "foo\xc2\x00password"); // stop at NULL
$test->ffi->tb_set_cell(0, $y++, 0xffff, 0, 0); // invalid codepoint

$test->ffi->tb_present();

$test->screencap();
