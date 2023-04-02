<?php
declare(strict_types=1);

return (function() {
    // This is a little whacky. `FFI::cdef` isn't smart enough to handle
    // preprocessor directives, so we feed it a `gcc -E` version of
    // termbox2.h (termbox2.ffi.h, created by the Makefile). On the other
    // hand, it's useful to have `#define` constants for tests, so we parse
    // those out from the raw  `termbox2.h`.
    $repo_dir = dirname(__DIR__);
    $termbox_h = "$repo_dir/termbox2.h";
    $termbox_ffi_h = "$repo_dir/termbox2.ffi.h";
    $libtermbox_so = "$repo_dir/libtermbox2.so";
    $termbox_h_data = file_get_contents($termbox_h);

    // Look at only the content in between `__TERMBOX_H`
    $matches = [];
    preg_match(
        '@#define __TERMBOX_H\n(.*?)#endif /\* __TERMBOX_H \*/@sm',
        $termbox_h_data,
        $matches
    );
    $termbox_h_data = $matches[1] ?? '';

    // Extract #define values
    $defines = [];
    $matches = [];
    preg_match_all('/^#define\s+(TB_\S+)\s+(.+)$/m', $termbox_h_data, $matches, PREG_SET_ORDER);
    foreach ($matches as $match) {
        $define_name = $match[1];
        $define_value = $match[2];

        // Remove comments
        $define_value = trim(preg_replace('|/\*.*$|', '', $define_value));

        // Special case for evaluating `(0xFFFF - ...)` values
        $match2 = [];
        if (preg_match('/^\(0xffff - (\d+)\)$/i', $define_value, $match2)) {
            $define_value = 0xffff - (int)$match2[1];
        } else if (substr($define_value, 0, 2) === '0x') {
            $define_value = hexdec(substr($define_value, 2));
        }
        $defines[$define_name] = (int)$define_value;
    }

    // Make FFI
    $termbox_ffi_h_data = file_get_contents($termbox_ffi_h);
    $ffi = FFI::cdef($termbox_ffi_h_data, $libtermbox_so);

    return [ $ffi, $defines ];
})();
