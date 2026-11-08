<?php
declare(strict_types=1);

setlocale(LC_ALL, 'C.UTF-8');
$libc = FFI::cdef('int wcwidth(int); int iswprint(int);');

$test->ffi->tb_init();

$mismatch = 0;

// TODO: Remove when we upgrade the test container to Debian 14, which has
//       glibc 2.43, which supports Unicode 17.
$is_changed_in_unicode_17 = function ($c) {
    $ranges = [
        [ 0x00088f, 0x00088f ], [ 0x000c5c, 0x000c5c ], [ 0x000cdc, 0x000cdc ],
        [ 0x001acf, 0x001add ], [ 0x001ae0, 0x001aeb ], [ 0x0020c1, 0x0020c1 ],
        [ 0x002b96, 0x002b96 ], [ 0x00a7ce, 0x00a7cf ], [ 0x00a7d2, 0x00a7d2 ],
        [ 0x00a7d4, 0x00a7d4 ], [ 0x00a7f1, 0x00a7f1 ], [ 0x00fbc3, 0x00fbd2 ],
        [ 0x00fd90, 0x00fd91 ], [ 0x00fdc8, 0x00fdce ], [ 0x010940, 0x010959 ],
        [ 0x010ec5, 0x010ec7 ], [ 0x010ed0, 0x010ed8 ], [ 0x010efa, 0x010efb ],
        [ 0x011b60, 0x011b67 ], [ 0x011db0, 0x011ddb ], [ 0x011de0, 0x011de9 ],
        [ 0x016ea0, 0x016eb8 ], [ 0x016ebb, 0x016ed3 ], [ 0x016ff2, 0x016ff6 ],
        [ 0x0187f8, 0x0187ff ], [ 0x018d09, 0x018d1e ], [ 0x018d80, 0x018df2 ],
        [ 0x01ccfa, 0x01ccfc ], [ 0x01ceba, 0x01ced0 ], [ 0x01cee0, 0x01cef0 ],
        [ 0x01e6c0, 0x01e6de ], [ 0x01e6e0, 0x01e6f5 ], [ 0x01e6fe, 0x01e6ff ],
        [ 0x01f6d8, 0x01f6d8 ], [ 0x01f777, 0x01f77a ], [ 0x01f8d0, 0x01f8d8 ],
        [ 0x01fa54, 0x01fa57 ], [ 0x01fa8a, 0x01fa8a ], [ 0x01fa8e, 0x01fa8e ],
        [ 0x01fac8, 0x01fac8 ], [ 0x01facd, 0x01facd ], [ 0x01faea, 0x01faea ],
        [ 0x01faef, 0x01faef ], [ 0x01fbfa, 0x01fbfa ], [ 0x02b73a, 0x02b73f ],
        [ 0x02cea2, 0x02cead ], [ 0x0323b0, 0x033479 ]
    ];
    foreach ($ranges as [$s, $e]) if ($c >= $s && $c <= $e) return true;
    return false;
};

$test->ffi->quiet(true);

for ($c = 0; $c <= 0x10ffff; $c++) {
    if ($test->ffi->tb_wcwidth($c) !== $libc->wcwidth($c)) {
        if ($is_changed_in_unicode_17($c)) continue;
        ++$mismatch;
    }
    if ((bool)$test->ffi->tb_iswprint($c) !== (bool)$libc->iswprint($c)) {
        if ($is_changed_in_unicode_17($c)) continue;
        ++$mismatch;
    }
}
$test->ffi->quiet(false);

$test->ffi->tb_printf(0, 0, 0, 0, "mismatch=%d", $mismatch);

$test->ffi->tb_present();

$test->screencap();
