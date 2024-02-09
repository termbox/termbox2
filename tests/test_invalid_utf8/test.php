<?php
declare(strict_types=1);

$test->ffi->tb_init();
$test->ffi->tb_print_ex(0, 0, 0, 0, NULL, "foo\xc2\x00password");
$test->ffi->tb_present();

$test->screencap();
