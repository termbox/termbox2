<?php
declare(strict_types=1);

if ($test->ffi->tb_attr_width() !== 64) {
    // This will only work with 64-bit attrs
    $test->skip();
}

$test->ffi->tb_init();

$attrs = [
    'TB_STRIKEOUT',
    // 'TB_OVERLINE', // Not supported by xterm
    'TB_INVISIBLE',
    'TB_UNDERLINE_2',
];

$y = 0;
foreach ($attrs as $attr) {
    $test->ffi->tb_printf(0, $y++, $test->defines[$attr], 0, 'attr=%s', $attr);
}

$test->ffi->tb_present();

$test->screencap();
