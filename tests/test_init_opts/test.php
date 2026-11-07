<?php
declare(strict_types=1);

// Invalid option structures must be rejected without initializing termbox.
$null_err = $test->ffi->tb_init_ex(FFI::cast("struct tb_init_opts *", 0));
$opts = $test->ffi->new("struct tb_init_opts");
$size_field = $test->ffi->new("size_t");
$opts->size = FFI::sizeof($size_field);
$short_err = $test->ffi->tb_init_ex(FFI::addr($opts));
$opts->size = FFI::sizeof($opts);
$opts->file = "/dev/tty";
$init_err = $test->ffi->tb_init_ex(FFI::addr($opts));

$test->ffi->tb_printf(0, 0, 0, 0, "null=%d short=%d init=%d", $null_err, $short_err, $init_err);
$test->ffi->tb_present();
$test->screencap();
