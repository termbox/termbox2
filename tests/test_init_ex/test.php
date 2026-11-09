<?php
declare(strict_types=1);

$libc = FFI::cdef(
    'int open(const char *path, int flags, ...);' .
    'int close(int fd);'
);

$make_init_opts = function(callable $fn) use ($test) {
    $o = $test->ffi->new('struct tb_init_opts');
    $size = FFI::sizeof($o);
    FFI::memset($o, 0, $size);
    $o->size = $size;
    $fn($o);
    return $o;
};

$rv = [];

// like tb_init_file
$o = $make_init_opts(function ($o) use ($test) {
    $o->ttypath = $test->ffi->new('char[10]', false);
    FFI::memcpy($o->ttypath, "/dev/null\x00", 10);
});
$rv[] = $test->ffi->tb_init_ex(FFI::addr($o));
$rv[] = $test->ffi->tb_shutdown();

// like tb_init_fd
$O_WRONLY = 0x01;
$fd = $libc->open('/dev/null', $O_WRONLY);
$o = $make_init_opts(fn ($o) => $o->ttyfd = $fd);
$rv[] = $test->ffi->tb_init_ex(FFI::addr($o));
$rv[] = $test->ffi->tb_shutdown();
$libc->close($fd);

// like tb_init_rwfd
$O_WRONLY = 0x01;
$fd = $libc->open('/dev/null', $O_WRONLY);
$o = $make_init_opts(fn ($o) => $o->rfd = $o->wfd = $fd);
$rv[] = $test->ffi->tb_init_ex(FFI::addr($o));
$rv[] = $test->ffi->tb_shutdown();
$libc->close($fd);

// like tb_init
$o = $make_init_opts(function ($o) {});
$rv[] = $test->ffi->tb_init_ex(FFI::addr($o));
$rv[] = $test->ffi->tb_shutdown();
$libc->close($fd);

$test->ffi->tb_init();
$test->ffi->tb_print(0, 0, 0, 0, implode(',', $rv));
$test->ffi->tb_present();
$test->screencap();
