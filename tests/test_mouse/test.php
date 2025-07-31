<?php
declare(strict_types=1);

$libc = FFI::cdef(
    'int memfd_create(const char *name, unsigned int flags);' .
    'int close(int fd);'
);
$ttyin = $libc->memfd_create('ttyin', 0);
$ttyout = $libc->memfd_create('ttyout', 0);

$fttyin = fopen("php://fd/$ttyin", 'w');
$fttyin_nbytes = 0;
$fttyin_write = function ($s, $rewind = false) use (&$fttyin, &$fttyin_nbytes) {
    $rewind = true;
    $nbytes = fwrite($fttyin, $s);
    $fttyin_nbytes += $nbytes;
    if ($rewind) {
        // fwrite(STDERR, "rewinding $fttyin_nbytes byte(s)\n");
        fseek($fttyin, $fttyin_nbytes * -1, SEEK_END);
        $fttyin_nbytes = 0;
    }
};

$key2str = function($key) use ($test) {
    return match ($key) {
        $test->defines['TB_KEY_MOUSE_LEFT']       => 'left',
        $test->defines['TB_KEY_MOUSE_RIGHT']      => 'right',
        $test->defines['TB_KEY_MOUSE_MIDDLE']     => 'middle',
        $test->defines['TB_KEY_MOUSE_RELEASE']    => 'release',
        $test->defines['TB_KEY_MOUSE_WHEEL_UP']   => 'wheel-up',
        $test->defines['TB_KEY_MOUSE_WHEEL_DOWN'] => 'wheel-down',
        default => "$key",
    };
};

$esc2str = function($str) {
    return preg_replace_callback(
        '/[^\x21-\x7e]/',
        fn ($m) => sprintf('<%s>', dechex(ord($m[0]))),
        $str
    );
};

// Init memfd-backed termbox
$test->ffi->tb_init_rwfd($ttyin, $ttyout);
$test->ffi->tb_set_input_mode($test->defines['TB_INPUT_ALT'] | $test->defines['TB_INPUT_MOUSE']);
$event = $test->ffi->new('struct tb_event');

// Send mouse events to ftty
$coords = [
    [ 0,    0    ],
    [ 222,  222  ],
    [ 1023, 1023 ],
];
$results = [];
foreach (['vt200', '1006', '1015'] as $mode) {
    foreach (['left', 'middle', 'right', 'release'] as $action) {
        foreach ([false, true] as $wheel) {
            foreach ([false, true] as $motion) {
                foreach ($coords as $coord) {
                    list ($x, $y) = $coord;

                    if ($wheel && !in_array($action, ['left', 'middle'])) {
                        continue;
                    } else if ($mode === 'vt200' && ($x > 0xff || $y > 0xff)) {
                        continue;
                    }

                    $b = 0;
                    $b |= match ($action) {
                        'left'    => 0b00,
                        'middle'  => 0b01,
                        'right'   => 0b10,
                        'release' => 0b11,
                    };
                    if ($wheel) {
                        $b |= 0x40;
                    }
                    if ($motion) {
                        $b |= 0x20;
                    }
                    if ($mode !== '1006') {
                        $b += 0x20;
                    }
                    $esc = match ($mode) {
                        'vt200' => sprintf("\x1b[M%s%s%s", chr($b), chr(0x21 + $x), chr(0x21 + $y)),
                        '1006'  => sprintf("\x1b[<%d;%d;%d%s", $b, $x + 1, $y + 1, $action === 'release' ? 'm' : 'M'),
                        '1015'  => sprintf("\x1b[%d;%d;%dM", $b, $x + 1 , $y + 1),
                    };

                    $esc_len = strlen($esc);
                    $esc_so_far = '';

                    for ($i = 0; $i < $esc_len; $i++) {
                        $esc_byte = substr($esc, $i, 1);
                        $esc_so_far .= $esc_byte;

                        $is_full = $i === $esc_len - 1;
                        $rewind_stream = $is_full;

                        $fttyin_write($esc_byte, $rewind_stream);

                        FFI::memset(FFI::addr($event), 0, FFI::sizeof($event));
                        $rv = $test->ffi->tb_peek_event(FFI::addr($event), 1000);

                        $result = sprintf(
                            '%20s %-5s %-7s %-5s %-6s %-7s %-9s rv=%-2d %-9s type=%-6s mod=%-6s key=%-10s',
                            $esc2str($esc_so_far),
                            $mode,
                            $action,
                            $wheel ? 'wheel' : '-',
                            $motion ? 'motion' : '-',
                            $is_full ? 'full' : '-',
                            "$x,$y",
                            $rv,
                            "{$event->x},{$event->y}",
                            ($event->type & $test->defines['TB_EVENT_MOUSE']) ? 'mouse' : "{$event->type}",
                            ($event->mod & $test->defines['TB_MOD_MOTION']) ? 'motion' : "{$event->mod}",
                            $key2str($event->key),
                        );

                        fwrite(STDERR, "{$result}\n");
                        $results[] = $result;
                    }
                }
            }
        }
    }
}
fwrite(STDERR, "\n");

// Shutdown
fclose($fttyin);
$libc->close($ttyin);
$libc->close($ttyout);
$test->ffi->tb_shutdown();

// Results are too large for 80x24,
// so print to stderr and emit a hash in the tty
$hash = md5(implode("\n", $results));
$test->ffi->tb_init();
$test->ffi->tb_printf(0, 0, 0, 0, "hash=%s", $hash);
$test->ffi->tb_present();
$test->screencap();
