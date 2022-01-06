<?php
declare(strict_types=1);

$test->ffi->tb_init();

$w = $test->ffi->tb_width();
$h = $test->ffi->tb_height();

// try to set a cell out of bounds
$err = $test->ffi->tb_set_cell(-1, -1, 'x', 0, 0);
$errmsg = $test->ffi->tb_strerror($err);

$test->ffi->tb_printf(0, 0, 0, 0, "oob err=%d errmsg=%s", $err, $errmsg);

$test->ffi->tb_present();

$test->screencap();
