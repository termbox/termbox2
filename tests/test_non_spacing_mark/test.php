<?php
declare(strict_types=1);

if (!$test->ffi->tb_has_egc()) {
    // This will only work with extended grapheme cluster support
    $test->skip();
}

$test->ffi->tb_init();

$test->ffi->tb_print(0, 0, 0, 0, "STARG\xce\x9b\xcc\x8aTE SG-1");
$test->ffi->tb_print(0, 1, 0, 0, "a = v\xcc\x87 = r\xcc\x88, a\xe2\x83\x91 \xe2\x8a\xa5 b\xe2\x83\x91");


$test->ffi->tb_present();
$test->screencap();
