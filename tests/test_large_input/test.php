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

// we're going to send a bunch of up-arrow escape sequences to termbox via our
// fake tty, and then ensure that termbox emits the expected up-arrow events
// via `tb_peek_event`. we will send enough data to fill up the read buffer
// (TB_OPT_READ_BUF).
$up_arrow = "\x1bOA";
$read_buf_size = $test->defines['TB_OPT_READ_BUF'];
$num_up_arrows = (int)ceil($read_buf_size / strlen($up_arrow));
$input_data = str_repeat($up_arrow, $num_up_arrows);
$fttyin = fopen("php://fd/$ttyin", 'w');
$nbytes = fwrite($fttyin, $input_data);
fseek($fttyin, strlen($input_data) * -1, SEEK_CUR);

// count how many up arrow events termbox emits
$up_arrow_count = 0;
$test->ffi->tb_set_input_mode($test->defines['TB_INPUT_ALT']);
$event = $test->ffi->new('struct tb_event');
do {
    $rv = $test->ffi->tb_peek_event(FFI::addr($event), 1000);
    if ($rv == 0 && $event->key === $test->defines['TB_KEY_ARROW_UP']) {
        $up_arrow_count += 1;
    }
} while ($rv == 0);

// close fake termbox setup
fclose($fttyin);
$libc->close($ttyin);
$libc->close($ttyout);
$test->ffi->tb_shutdown();

// display up_arrow_count
$test->ffi->tb_init();
$test->ffi->tb_printf(0, 0, 0, 0, "up_arrow_count=%d", $up_arrow_count);
$test->ffi->tb_present();
$test->screencap();
