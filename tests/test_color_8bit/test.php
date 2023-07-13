<?php
declare(strict_types=1);

$test->ffi->tb_init();

$w = $test->ffi->tb_width();
$h = $test->ffi->tb_height();

$x = 0;
$y = 0;

function test_mode($test, string $mode, int $n, int $w, int &$x, int &$y): void {
    $attr_default = $test->defines['TB_DEFAULT'];
    $attr_italic = $test->defines['TB_ITALIC'];
    $attr_reverse = $test->defines['TB_REVERSE'];

    $test->ffi->tb_set_output_mode($test->defines[$mode]);
    $test->ffi->tb_print($x = 0, $y, $attr_default, $attr_default, $mode);

    $y++;
    for ($fg = 0; $fg <= $n; $fg++) {
        $s = "\xe2\x96\x80";
        $slen = 1; // mb_strlen
        if ($x + $slen > $w) {
            $x = 0;
            $y++;
        }
        if ($mode === 'TB_OUTPUT_256') {
            if ($fg === 0) {
                $cfg = $attr_default;
            } else if ($fg === 1) {
                $cfg = $test->defines['TB_HI_BLACK'];
            } else {
                $cfg = $fg - 1;
            }
        } else {
            $cfg = $fg;
        }
        $test->ffi->tb_print($x, $y, $cfg, $attr_default, $s);
        $x += $slen;
    }

    $y++;
    $test->ffi->tb_print($x = 0, $y++, $attr_default | $attr_italic, 6, "fg=def|ital bg=6");
    $test->ffi->tb_print($x = 0, $y++, 0             | $attr_italic, 6, "fg=0x0|ital bg=6");

    $test->ffi->tb_present();
}

// TB_OUTPUT_NORMAL
test_mode($test, 'TB_OUTPUT_NORMAL',    8,   $w, $x, $y);
test_mode($test, 'TB_OUTPUT_256',       256, $w, $x, $y);
test_mode($test, 'TB_OUTPUT_216',       216, $w, $x, $y);
test_mode($test, 'TB_OUTPUT_GRAYSCALE', 24,  $w, $x, $y);

$test->screencap();
