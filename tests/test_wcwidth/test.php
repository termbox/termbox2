<?php
declare(strict_types=1);

setlocale(LC_ALL, 'C.UTF-8');
$libc = FFI::cdef('int wcwidth(int); int iswprint(int);');

$test->ffi->tb_init();

$mismatch = 0;

$test->ffi->quiet(true);
for ($c = 0; $c <= 0x10ffff; $c++) {
    if ($test->ffi->tb_wcwidth($c) !== $libc->wcwidth($c)) {
        ++$mismatch;
    }
    if ((bool)$test->ffi->tb_iswprint($c) !== (bool)$libc->iswprint($c)) {
        ++$mismatch;
    }
}
$test->ffi->quiet(false);

$test->ffi->tb_printf(0, 0, 0, 0, "mismatch=%d", $mismatch);

$test->ffi->tb_present();

$test->screencap();
