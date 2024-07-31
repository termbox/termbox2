<?php
declare(strict_types=1);

$test->ffi->tb_init();

$codepoints = [
    0x00, // NULL
    0x01, // control code
    0x08, // backspace
    0x09, // tab
    0x0a, // newline
    0x1f, // control code
    0x7f, // delete
];

$y = 0;
foreach ($codepoints as $ch) {
    $test->ffi->tb_printf(0, $y, 0, 0, "0x%02x ", $ch);
    $test->ffi->tb_set_cell(5, $y, $ch, 0, 0);
    $y += 1;
}

$test->ffi->tb_present();

$test->screencap();
