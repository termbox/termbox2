#!/usr/bin/env php
<?php declare(strict_types=1);

(new class {
    private function usage(int $exit_code): void {
        printf("Usage: %s <terminfo_h|h|terminfo_c|c|wcwidth|w>\n", __FILE__);
        exit($exit_code);
    }

    private const BUILTIN_TERMS = [
        [ 'xterm',         null   ],
        [ 'linux',         null   ],
        [ 'screen',        'tmux' ],
        [ 'rxvt-256color', null   ],
        [ 'rxvt-unicode',  'rxvt' ],
        [ 'Eterm',         null   ],
    ];

    private const TERMINFO_KEYS = [
        'kf1'   => 'F1',
        'kf2'   => 'F2',
        'kf3'   => 'F3',
        'kf4'   => 'F4',
        'kf5'   => 'F5',
        'kf6'   => 'F6',
        'kf7'   => 'F7',
        'kf8'   => 'F8',
        'kf9'   => 'F9',
        'kf10'  => 'F10',
        'kf11'  => 'F11',
        'kf12'  => 'F12',
        'kich1' => 'INSERT',
        'kdch1' => 'DELETE',
        'khome' => 'HOME',
        'kend'  => 'END',
        'kpp'   => 'PGUP',
        'knp'   => 'PGDN',
        'kcuu1' => 'ARROW_UP',
        'kcud1' => 'ARROW_DOWN',
        'kcub1' => 'ARROW_LEFT',
        'kcuf1' => 'ARROW_RIGHT',
        'kcbt'  => 'BACK_TAB',
    ];

    private const TERMINFO_FUNCS = [
        'smcup' => 'ENTER_CA',
        'rmcup' => 'EXIT_CA',
        'cnorm' => 'SHOW_CURSOR',
        'civis' => 'HIDE_CURSOR',
        'clear' => 'CLEAR_SCREEN',
        'sgr0'  => 'SGR0',
        'smul'  => 'UNDERLINE',
        'bold'  => 'BOLD',
        'blink' => 'BLINK',
        'sitm'  => 'ITALIC',
        'rev'   => 'REVERSE',
        'smkx'  => 'ENTER_KEYPAD',
        'rmkx'  => 'EXIT_KEYPAD',
        'dim'   => 'DIM',
        'invis' => 'INVISIBLE',
    ];

    private const EXTRA_KEYS = [
        'MOUSE_LEFT',
        'MOUSE_RIGHT',
        'MOUSE_MIDDLE',
        'MOUSE_RELEASE',
        'MOUSE_WHEEL_UP',
        'MOUSE_WHEEL_DOWN',
    ];

    public function run(): void {
        match ($GLOBALS['argv'][1] ?? null) {
            'terminfo_h', 'h' => $this->genTerminfoHeader(),
            'terminfo_c', 'c' => $this->genTerminfoStructs(),
            'wcwidth', 'w' => $this->genWcwidthTable(),
            null, '-h', '--help' => $this->usage(0),
            default => $this->usage(1),
        };
    }

    private function genTerminfoHeader(): void {
        $this->genTerminfo(false);
    }

    private function genTerminfoStructs(): void {
        $this->genTerminfo(true);
    }

    private function genTerminfo(bool $structs): void {
        // codegen terminfo_cap_indexes
        // codegen #define TB_CAP_*
        // codegen #define TB_KEY_*
        $c_cap_indexes = [];
        $c_cap_defines = [];
        $c_cap_num = 0;
        $c_key_num = 0;

        $c_cap_indexes[] = 'static const int16_t terminfo_cap_indexes[] = {';

        foreach (self::TERMINFO_KEYS as $string_name => $define_name) {
            $string_index = $this->getTerminfoStringIndex($string_name);
            $c_cap_indexes[] = "    $string_index, // $string_name (TB_CAP_{$define_name})";
            $c_cap_defines[] = "#define TB_CAP_{$define_name} $c_cap_num";
            $c_key_defines[] = "#define TB_KEY_{$define_name} (0xffff - $c_key_num)";
            $c_cap_num += 1;
            $c_key_num += 1;
        }

        $c_cap_defines[] = "#define TB_CAP__COUNT_KEYS $c_cap_num";
        foreach (self::EXTRA_KEYS as $define_name) {
            $c_key_defines[] = "#define TB_KEY_{$define_name} (0xffff - $c_key_num)";
            $c_key_num += 1;
        }

        foreach (self::TERMINFO_FUNCS as $string_name => $define_name) {
            $string_index = $this->getTerminfoStringIndex($string_name);
            $c_cap_indexes[] = "    $string_index, // $string_name (TB_CAP_{$define_name})";
            $c_cap_defines[] = "#define TB_CAP_{$define_name} $c_cap_num";
            $c_cap_num += 1;
        }

        $c_cap_defines[] = "#define TB_CAP__COUNT $c_cap_num";
        $c_cap_indexes[] = '};';
        $c_cap_indexes[] = '';

        $c_codegen_comment = sprintf('/* Produced by %s on %s */', basename(__FILE__), gmdate('r'));

        if ($structs) {
            echo "$c_codegen_comment\n\n";
            echo implode("\n", $c_cap_indexes) . "\n";

            // codegen built-in _keys and _funcs as _caps
            $c_term_name_cap = [];
            $c_term_name_cap[] = 'static struct {';
            $c_term_name_cap[] = '    const char *name;';
            $c_term_name_cap[] = '    const char **caps;';
            $c_term_name_cap[] = '    const char *alias;';
            $c_term_name_cap[] = '} builtin_terms[] = {';
            foreach (self::BUILTIN_TERMS as [ $term_name, $term_alias ]) {
                $c_term_name = preg_replace('/[^a-z0-9]/', '_', strtolower($term_name));

                $c_term_caps = [];
                $c_term_caps[] = "static const char *{$c_term_name}_caps[] = {";
                foreach (
                    array_merge(self::TERMINFO_KEYS, self::TERMINFO_FUNCS) as
                    $string_name => $define_name
                ) {
                    $c_string_literal = $this->getTerminfoStringLiteral($term_name, $string_name);
                    $c_term_caps[] = "    {$c_string_literal}, // $string_name (TB_CAP_{$define_name})";
                }
                $c_term_caps[] = '};';
                $c_term_caps[] = '';

                echo "// $term_name\n";
                echo implode("\n", $c_term_caps) . "\n";

                $c_term_name_cap[] = "    { \"{$term_name}\", {$c_term_name}_caps, \"{$term_alias}\" },";
            }
            $c_term_name_cap[] = '    { NULL, NULL, NULL },';
            $c_term_name_cap[] = '};';

            echo implode("\n", $c_term_name_cap) . "\n\n";
        } else {
            echo "$c_codegen_comment\n";
            echo implode("\n", $c_key_defines) . "\n\n";
            echo implode("\n", $c_cap_defines) . "\n";
        }
    }

    private function getTerminfoStringIndex(string $string_name): int {
        $cmd = sprintf(
            "infocmp -E | grep -w %s | awk '{print \$2}' | sed 's|:\$||g'",
            escapeshellarg($string_name)
        );
        $index = trim(shell_exec($cmd) ?: '');
        if (empty($index)) {
            throw new RuntimeException("getTerminfoStringIndex failed for $string_name");
        }
        return (int)$index;
    }

    private function getTerminfoStringLiteral(string $term_name, string $string_name): string {
        $cmd = sprintf(
            "infocmp -E %s | grep -w %s | head -n1 | awk '{print \$5}' | sed 's|,\$||g'",
            escapeshellarg($term_name),
            escapeshellarg($string_name)
        );
        $var_name = trim(shell_exec($cmd) ?: '');
        if ($var_name === 'ABSENT_STRING' || $var_name === '') {
            return '""';
        }
        $cmd2 = sprintf(
            "infocmp -E %s | grep -w %s | grep ' = ' | head -n1 | awk '{print \$NF}' | sed 's|;\$||g'",
            escapeshellarg($term_name),
            escapeshellarg($var_name)
        );
        $literal = trim(shell_exec($cmd2) ?: '');
        if (empty($literal)) {
            return '""';
        }
        return $literal;
    }

    private function genWcwidthTable(): void {
        setlocale(LC_ALL, 'C.UTF-8');
        $libc = FFI::cdef(
            'int wcwidth(int);' .
            'int iswprint(int);'
        );
        $range_start = null;
        $range_val = null;
        $c = 1;
        $n = 0;
        $emit = function () use (&$range_start, &$c, &$range_val, &$n) {
            if ($n % 3 === 0) echo '    ';
            printf("{0x%06x, 0x%06x, %2d},", $range_start, $c - 1, $range_val);
            echo ($n % 3 === 2) ? "\n" : ' ';
            ++$n;
        };
        for ($c = 1; $c <= 0x10ffff; $c++) {
            $w = $libc->wcwidth($c);
            $p = $libc->iswprint($c);
            $val = $p ? $w : -1;
            if ($range_val !== $val) {
                if ($range_val !== null) $emit();
                $range_start = $c;
                $range_val = $val;
            }
        }
        $emit();
        if (($n - 1) % 3 !== 2) echo "\n";
        echo "#define WCWIDTH_TABLE_LENGTH $n\n";
    }
})->run();
