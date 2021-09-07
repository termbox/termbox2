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
        $repo_dir = dirname(__DIR__);
        $termbox_h = "$repo_dir/termbox.h";
        $libtermbox_so = "$repo_dir/libtermbox.so";
        $header_data_and_impl = file_get_contents($termbox_h);

        $matches = [];
        preg_match(
            '@#define __TERMBOX_H\n(.*?)#endif /\* __TERMBOX_H \*/@sm',
            $header_data_and_impl,
            $matches
        );
        $header_data = $matches[1] ?? '';

        // Extract #define values
        $matches = [];
        preg_match_all('/^#define\s+(TB_\S+)\s+(.+)$/m', $header_data, $matches, PREG_SET_ORDER);
        foreach ($matches as $match) {
            $define_name = $match[1];
            $define_value = $match[2];

            // Remove comments
            $define_value = trim(preg_replace('|/\*.*$|', '', $define_value));

            // Special case for evaluating `(0xFFFF - ...)` values
            $match2 = [];
            if (preg_match('/^\(0xFFFF - (\d+)\)$/', $define_value, $match2)) {
                $define_value = 0xFFFF - (int)$match[2];
            } else if (substr($define_value, 0, 2) === '0x') {
                $define_value = hexdec(substr($define_value, 2));
            }
            $this->defines[$define_name] = (int)$define_value;
        }

        // Make FFI
        $header_data = preg_replace('/^.*__ffi_strip.*$/m', '', $header_data);
        $ffi = FFI::cdef($header_data, $libtermbox_so);

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
        $lines = explode("\n", $str);
        foreach ($lines as $line) {
            file_put_contents($this->test_log, "  $line\n", FILE_APPEND);
        }
    }

    public function screencap(): void {
        $this->log('screencap');
        sleep(PHP_INT_MAX);
    }
};
