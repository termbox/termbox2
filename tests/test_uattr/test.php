<?php
declare(strict_types=1);

if ($test->ffi->tb_attr_width() !== 64) {
    $test->skip();
}

$test->ffi->tb_init();

$uattr = $test->defines['TB_UATTR'];
$red = $test->defines['TB_RED'];
$bold = $test->defines['TB_BOLD'];
$uattr_start = $test->defines['TB_UATTR_START'];
$curly  = $uattr_start << 0;
$dotted = $uattr_start << 1;
$dashed = $uattr_start << 2;

$test->ffi->tb_set_uattr_func(function ($fg, $bg, $out, $nout) use ($test, $curly, $dotted, $dashed) {
    $code = '';
    if      ($fg & $curly)  $code = "\x1b[4:3m";
    else if ($fg & $dotted) $code = "\x1b[4:4m";
    else if ($fg & $dashed) $code = "\x1b[4:5m";
    $code_len = strlen($code);
    $buf = $test->ffi->new('char[64]', false);
    FFI::memcpy($buf, $code, $code_len);
    $out[0] = $buf;
    $nout[0] = $code_len;
    return $test->defines['TB_OK'];
});

$y = 0;
$test->ffi->tb_print(0, $y++, $red | $bold,                                 0, 'red and bold');
$test->ffi->tb_print(0, $y++, $test->defines['TB_UATTR'] | $curly,          0, 'curly and normal');
$test->ffi->tb_print(0, $y++, $test->defines['TB_UATTR'] | $dotted | $red,  0, 'dotted and red');
$test->ffi->tb_print(0, $y++, $test->defines['TB_UATTR'] | $dashed | $bold, 0, 'dashed and bold');

$test->ffi->tb_present();

$test->screencap();
