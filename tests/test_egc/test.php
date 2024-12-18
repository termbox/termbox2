<?php
declare(strict_types=1);

if (!$test->ffi->tb_has_egc()) {
    // This will only work with extended grapheme cluster support
    $test->skip();
}

$test->ffi->tb_init();

$ech = $test->ffi->new('uint32_t[2]');
$test->ffi->tb_utf8_char_to_unicode(FFI::addr($ech[0]), "\xd0\xb5"); // cyrillic small letter ie
$test->ffi->tb_utf8_char_to_unicode(FFI::addr($ech[1]), "\xcc\x88"); // combining diaeresis

// Set EGC using tb_set_cell_ex
$y = 0;
$test->ffi->tb_set_cell(0, $y, ord('a'), 0, 0);
$test->ffi->tb_set_cell_ex(1, $y, $test->ffi->cast('uint32_t *', FFI::addr($ech)), 2, 0, 0);
$test->ffi->tb_set_cell(2, $y, ord('b'), 0, 0);

// Same effect with tb_extend_cell
++$y;
$test->ffi->tb_set_cell(0, $y, ord('c'), 0, 0);
$test->ffi->tb_set_cell(1, $y, $ech[0], 0, 0);
$test->ffi->tb_extend_cell(1, $y, $ech[1]);
$test->ffi->tb_set_cell(2, $y, ord('d'), 0, 0);

// Another example with 2 combining characters
++$y;
$test->ffi->tb_set_cell(0, $y, 0xff29, 0, 0); // fullwidth latin capital letter i
$test->ffi->tb_extend_cell(0, $y, 0x0324); // combining diaeresis below
$test->ffi->tb_extend_cell(0, $y, 0x036f); // combining latin small letter x

// Same as above via tb_print (should be equivalent)
++$y;
$test->ffi->tb_print(0, $y, 0, 0, "\xef\xbc\xa9\xcc\xa4\xcd\xaf");

$test->ffi->tb_present();
$test->screencap();
