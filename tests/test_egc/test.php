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

++$y;
// Same effect with tb_extend_cell
$test->ffi->tb_set_cell(0, $y, ord('c'), 0, 0);
$test->ffi->tb_set_cell(1, $y, $ech[0], 0, 0);
$test->ffi->tb_extend_cell(1, $y, $ech[1]);
$test->ffi->tb_set_cell(2, $y, ord('d'), 0, 0);

$test->ffi->tb_present();
$test->screencap();
