<?php
declare(strict_types=1);

// Ensure we don't leak an fd when a non-tty is passed to `tb_init_file`.
// In this test we pass a memfd.
$libc = FFI::cdef(
    'int memfd_create(const char *name, unsigned int flags);' .
    'int close(int fd);'
);
$memfd = $libc->memfd_create('rw', 0);

$num_fds_i = count(glob('/proc/self/fd/*'));
$test->ffi->tb_init_file("/proc/self/fd/{$memfd}");
$test->ffi->tb_shutdown();
$num_fds_f = count(glob('/proc/self/fd/*'));
$num_fds_leaked = $num_fds_f - $num_fds_i;

$libc->close($memfd);

$test->ffi->tb_init();
$test->ffi->tb_printf(0, 0, 0, 0, "num_fds_leaked=%d", $num_fds_leaked);
$test->ffi->tb_present();

$test->screencap();
