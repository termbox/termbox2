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
        // Make FFI
        [ $ffi, $defines ] = require __DIR__ . '/test_ffi.php';
        $this->defines = $defines;

        // Return wrapper that logs FFI calls
        return new class($ffi, $this) {
            private FFI $ffi;
            private object $test;
            public function __construct($ffi, $test) {
                $this->ffi = $ffi;
                $this->test = $test;
            }
            public function __call(string $name, array $args) {
                $this->test->log("ffi $name " . json_encode($args));
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
