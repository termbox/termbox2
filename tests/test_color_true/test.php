<?php
declare(strict_types=1);

if (!$test->ffi->tb_has_truecolor()) {
    // This will only work with truecolor support
    $test->skip();
}

$attr_bold = $test->defines['TB_BOLD'];
$attr_underline = $test->defines['TB_UNDERLINE'];
$attr_italic = $test->defines['TB_ITALIC'];
$attr_default = $test->defines['TB_DEFAULT'];
$true_black = $test->defines['TB_HI_BLACK'];

$css_colors = [
    'antiquewhite'         => [ 0xfaebd7, $attr_default ],
    'aquamarine'           => [ 0x7fffd4, $attr_default ],
    'beige'                => [ 0xf5f5dc, $attr_default ],
    'black'                => [ $true_black, $attr_default ],
    'blue'                 => [ 0x0000ff, $attr_default ],
    'brown'                => [ 0xa52a2a, $attr_default ],
    'cadetblue'            => [ 0x5f9ea0, $attr_default ],
    'chocolate'            => [ 0xd2691e, $attr_default ],
    'cornflowerblue'       => [ 0x6495ed, $attr_default ],
    'crimson'              => [ 0xdc143c, $attr_default ],
    'darkblue'             => [ 0x00008b, $attr_default ],
    'darkgoldenrod'        => [ 0xb8860b, $attr_default ],
    'darkgreen'            => [ 0x006400, $attr_default ],
    'darkkhaki'            => [ 0xbdb76b, $attr_default ],
    'darkolivegreen'       => [ 0x556b2f, $attr_default ],
    'darkorchid'           => [ 0x9932cc, $attr_default ],
    'darksalmon'           => [ 0xe9967a, $attr_default ],
    'darkslateblue'        => [ 0x483d8b, $attr_default ],
    'darkturquoise'        => [ 0x00ced1, $attr_default ],
    'deeppink'             => [ 0xff1493, $attr_default ],
    'dimgray'              => [ 0x696969, $attr_default ],
    'dodgerblue'           => [ 0x1e90ff, $attr_default ],
    'floralwhite'          => [ 0xfffaf0, $attr_default ],
    'fuchsia'              => [ 0xff00ff, $attr_default ],
    'ghostwhite'           => [ 0xf8f8ff, $attr_default ],
    'goldenrod'            => [ 0xdaa520, $attr_default ],
    'green'                => [ 0x008000, $attr_default ],
    'grey'                 => [ 0x808080, $attr_default ],
    'hotpink'              => [ 0xff69b4, $attr_default ],
    'indigo'               => [ 0x4b0082, $attr_default ],
    'khaki'                => [ 0xf0e68c, $attr_default ],
    'lavenderblush'        => [ 0xfff0f5, $attr_default ],
    'lemonchiffon'         => [ 0xfffacd, $attr_default ],
    'lightcoral'           => [ 0xf08080, $attr_default ],
    'lightgoldenrodyellow' => [ 0xfafad2, $attr_default ],
    'lightgreen'           => [ 0x90ee90, $attr_default ],
    'lightpink'            => [ 0xffb6c1, $attr_default ],
    'lightseagreen'        => [ 0x20b2aa, $attr_default ],
    'lightslategrey'       => [ 0x778899, $attr_default ],
    'lightyellow'          => [ 0xffffe0, $attr_default ],
    'limegreen'            => [ 0x32cd32, $attr_default ],
    'magenta'              => [ 0xff00ff, $attr_default ],
    'mediumaquamarine'     => [ 0x66cdaa, $attr_default ],
    'mediumorchid'         => [ 0xba55d3, $attr_default ],
    'mediumseagreen'       => [ 0x3cb371, $attr_default ],
    'mediumspringgreen'    => [ 0x00fa9a, $attr_default ],
    'mediumvioletred'      => [ 0xc71585, $attr_default ],
    'mintcream'            => [ 0xf5fffa, $attr_default ],
    'moccasin'             => [ 0xffe4b5, $attr_default ],
    'navy'                 => [ 0x000080, $attr_default ],
    'olive'                => [ 0x808000, $attr_default ],
    'orange'               => [ 0xffa500, $attr_default ],
    'orchid'               => [ 0xda70d6, $attr_default ],
    'palegreen'            => [ 0x98fb98, $attr_default ],
    'palevioletred'        => [ 0xdb7093, $attr_default ],
    'peachpuff'            => [ 0xffdab9, $attr_default ],
    'pink'                 => [ 0xffc0cb, $attr_default ],
    'powderblue'           => [ 0xb0e0e6, $attr_default ],
    'red'                  => [ 0xff0000, $attr_default ],
    'royalblue'            => [ 0x4169e1, $attr_default ],
    'salmon'               => [ 0xfa8072, $attr_default ],
    'seagreen'             => [ 0x2e8b57, $attr_default ],
    'sienna'               => [ 0xa0522d, $attr_default ],
    'skyblue'              => [ 0x87ceeb, $attr_default ],
    'slategrey'            => [ 0x708090, $attr_default ],
    'springgreen'          => [ 0x00ff7f, $attr_default ],
    'tan'                  => [ 0xd2b48c, $attr_default ],
    'thistle'              => [ 0xd8bfd8, $attr_default ],
    'turquoise'            => [ 0x40e0d0, $attr_default ],
    'wheat'                => [ 0xf5deb3, $attr_default ],
    'whitesmoke'           => [ 0xf5f5f5, $attr_default ],
    'yellowgreen'          => [ 0x9acd32, $attr_default ],
    'green on default'     => [ 0x00ff00, $attr_default ],
    'black on default'     => [ $true_black, $attr_default ],
    'default on green'     => [ $attr_default, 0x00ff00 ],
    'default on black'     => [ $attr_default, $true_black ],
    'default on default'   => [ $attr_default, $attr_default ],
    'black on black w/ ignored bits' => [ 0x123456 | $true_black, 0x789abc | $true_black ],
    'italic on default'    => [ $attr_default | $attr_italic, $attr_default ],
];

$test->ffi->tb_init();

$w = $test->ffi->tb_width();
$h = $test->ffi->tb_height();

$test->ffi->tb_set_output_mode($test->defines['TB_OUTPUT_TRUECOLOR']);

// Test true colors
$x = 0;
$y = 0;
foreach ($css_colors as $name => [$fg, $bg]) {
    $s = "{$name}, ";
    $slen = strlen($s);
    if ($x + $slen > $w) {
        $x = 0;
        $y++;
    }
    $test->ffi->tb_print($x, $y, $fg, $bg, $s);
    $x += $slen;
}

// Test bold, underline, italic in true-color mode
$x = 0;
$color = 0x808080 | $attr_bold;
$test->ffi->tb_printf($x, ++$y, $color, $attr_default, 'yes bold (#%06x)', $color);
$color = 0x808080 | $attr_underline;
$test->ffi->tb_printf($x, ++$y, $color, $attr_default, 'yes underline (#%06x)', $color);
$color = 0x808080 | $attr_italic;
$test->ffi->tb_printf($x, ++$y, $color, $attr_default, 'yes italic (#%06x)', $color);

// Test fg/bg together
$x = 0;
$y += 1;
for ($r = 0x00; $r <= 0xff; $r += 0xff) {
    for ($g = 0x00; $g <= 0xff; $g += 0xff) {
        for ($b = 0x00; $b <= 0xff; $b += 0xff) {
            $fg = ($r << 16) + ($g << 8) + $b;
            $bg = ((0xff - $r) << 16) + ((0xff - $g) << 8) + (0xff - $b);
            $str = sprintf('#%06x on #%06x ', $fg, $bg);
            $slen = strlen($str);
            if ($x + $slen > $w) {
                $x = 0;
                $y++;
            }
            if ($fg == 0) $fg = $true_black;
            if ($bg == 0) $bg = $true_black;
            $test->ffi->tb_print($x, $y, $fg, $bg, $str);
            $x += $slen;
        }
    }
}

$test->ffi->tb_present();

$test->screencap();
