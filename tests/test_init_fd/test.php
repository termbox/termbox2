<?php
declare(strict_types=1);

$libc = FFI::cdef(
    'int open(const char *path, int flags, ...);' .
    'int close(int fd);'
);
$rv = [];

// tb_init_fd
$O_WRONLY = 0x01;
$fd = $libc->open('/dev/null', $O_WRONLY);
$rv[] = $test->ffi->tb_init_fd($fd);
$rv[] = $test->ffi->tb_shutdown();
$libc->close($fd);

// tb_init_file
$path = tempnam('/tmp', 'test_init_') ?: '/bogus';
$rv[] = $test->ffi->tb_init_file($path);
$rv[] = $test->ffi->tb_shutdown();
unlink($path);

$test->ffi->tb_init();
$test->ffi->tb_print(0, 0, 0, 0, implode(',', $rv));
$test->ffi->tb_present();
$test->screencap();
