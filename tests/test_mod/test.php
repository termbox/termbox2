<?php
declare(strict_types=1);

// init termbox with a "fake" tty backed by memfds
$libc = FFI::cdef(
    'int memfd_create(const char *name, unsigned int flags);' .
    'int close(int fd);'
);
$ttyin = $libc->memfd_create('ttyin', 0);
$ttyout = $libc->memfd_create('ttyout', 0);
$test->ffi->tb_init_rwfd($ttyin, $ttyout);
$input_data =
    "\x1b[1;2A" . // TB_KEY_ARROW_UP, TB_MOD_SHIFT
    "\x1b[1;3A" . // TB_KEY_ARROW_UP, TB_MOD_ALT
    "\x1b[1;4A" . // TB_KEY_ARROW_UP, TB_MOD_ALT | TB_MOD_SHIFT
    "\x1b[1;5A" . // TB_KEY_ARROW_UP, TB_MOD_CTRL
    "\x1b[1;6A" . // TB_KEY_ARROW_UP, TB_MOD_CTRL | TB_MOD_SHIFT
    "\x1b[1;7A" . // TB_KEY_ARROW_UP, TB_MOD_CTRL | TB_MOD_ALT
    "\x1b[1;8A" ; // TB_KEY_ARROW_UP, TB_MOD_CTRL | TB_MOD_ALT | TB_MOD_SHIFT
$fttyin = fopen("php://fd/$ttyin", 'w');
$nbytes = fwrite($fttyin, $input_data);
fseek($fttyin, strlen($input_data) * -1, SEEK_CUR);

// record events that termbox emits
$events = [];
$test->ffi->tb_set_input_mode($test->defines['TB_INPUT_ALT']);
$e = $test->ffi->new('struct tb_event');
do {
    $rv = $test->ffi->tb_peek_event(FFI::addr($e), 1000);
    if ($rv == 0) {
        $events[] = [ $e->type, $e->mod, $e->key, $e->ch ];
    }
} while ($rv == 0);

// close fake termbox setup
fclose($fttyin);
$libc->close($ttyin);
$libc->close($ttyout);
$test->ffi->tb_shutdown();

// display events
$test->ffi->tb_init();
$y = 0;
foreach ($events as $e) {
    $test->ffi->tb_printf(0, $y++, 0, 0, "event=%s", implode(',', $e));
}
$test->ffi->tb_present();
$test->screencap();
