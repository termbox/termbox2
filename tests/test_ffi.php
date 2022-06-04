<?php
declare(strict_types=1);

$test = new class() {
    public object $ffi;
    public array $defines = [];
    private string $test_log;

    public function __construct() {
        setlocale(LC_ALL, '');
        $this->ffi = $this->makeFfi();
        $this->test_log = $GLOBALS['argv'][1] ?? '';
    }

    private function makeFfi(): object {
        // This is a little whacky. `FFI::cdef` isn't smart enough to handle
        // preprocessor directives, so we feed it a `gcc -E` version of
        // termbox.h (termbox.ffi.h, created by the Makefile). On the other
        // hand, it's useful to have `#define` constants for tests, so we parse
        // those out from the raw  `termbox.h`.
        $repo_dir = dirname(__DIR__);
        $termbox_h = "$repo_dir/termbox.h";
        $termbox_ffi_h = "$repo_dir/termbox.ffi.h";
        $libtermbox_so = "$repo_dir/libtermbox.so";
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
            $this->defines[$define_name] = (int)$define_value;
        }

        // Make FFI
        $termbox_ffi_h_data = file_get_contents($termbox_ffi_h);
        $ffi = FFI::cdef($termbox_ffi_h_data, $libtermbox_so);

        // Return wrapper that logs FFI calls
        return new class($ffi, $this) {
            private FFI $ffi;
            private object $test;
            public function __construct($ffi, $test) {
                $this->ffi = $ffi;
                $this->test = $test;
            }
            public function __call(string $name, array $args) {
                if ($name !== 'tb_set_cell') {
                    $this->test->log("ffi $name " . json_encode($args));
                }
                return $this->ffi->$name(...$args);
            }
        };
    }

    public function xvkbd(string $xvkbd_cmd): int {
        $this->log("xvkbd $xvkbd_cmd");
        $cmd = sprintf(
            "DISPLAY=:1000 xvkbd -remote-display :1000 -window xterm -text %s",
            escapeshellarg($xvkbd_cmd)
        );
        $sh_cmd = sprintf(
            'sh -c %s >/dev/null 2>&1',
            escapeshellarg($cmd)
        );
        $output = [];
        $exit_code = 1;
        exec($sh_cmd, $output, $exit_code);
        return $exit_code;
    }

    public function log(string $str): void {
        if (!$this->test_log) {
            return;
        }
        $lines = explode("\n", $str);
        foreach ($lines as $line) {
            file_put_contents($this->test_log, "  $line\n", FILE_APPEND);
        }
    }

    public function screencap(): void {
        $this->log('screencap');
        sleep(PHP_INT_MAX);
    }

    public function skip(): void {
        $this->log('skip');
        sleep(PHP_INT_MAX);
    }
};
