<?php
declare(strict_types=1);

putenv('TERM=invalid-term');

$test->ffi->tb_init();
$test->ffi->tb_shutdown();

file_put_contents("/dev/tty", "no_segfault\n");

$test->screencap();
