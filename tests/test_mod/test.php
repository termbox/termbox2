<?php
declare(strict_types=1);

$test->ffi->tb_init();

$w = $test->ffi->tb_width();
$h = $test->ffi->tb_height();

$y = 0;
foreach (['\[Up]'] as $key) {
    foreach ([false, true] as $mod_ctrl) {
        foreach ([false, true] as $mod_alt) {
            foreach ([false, true] as $mod_shift) {
                $xvkbd_cmd = '';
                if ($mod_ctrl)  $xvkbd_cmd .= '\C';
                if ($mod_alt)   $xvkbd_cmd .= '\A';
                if ($mod_shift) $xvkbd_cmd .= '\S';
                $xvkbd_cmd .= $key;
                $test->xvkbd($xvkbd_cmd);

                $event = $test->ffi->new('struct tb_event');
                $rv = $test->ffi->tb_peek_event(FFI::addr($event), 1000);

                $test->ffi->tb_printf(0, $y, 0, 0, "%16s -> key=%d mod=%d",
                    $xvkbd_cmd,
                    $event->key,
                    $event->mod,
                );
                $y += 1;
            }
        }
    }
}

$test->ffi->tb_present();

$test->screencap();
