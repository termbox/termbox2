<?php
declare(strict_types=1);

$term_h = (int)shell_exec('tput lines') ?: 24;

# Write some content before we init
$content = implode("\n", range(1, $term_h));
$fake_prompt = 'user@host:~ $';
file_put_contents('/dev/tty', "{$content}\n{$fake_prompt}\n");

$expanded = false;
$shrunk = false;
$fill_region = function() use ($test, &$expanded, &$shrunk) {
    $test->ffi->tb_clear();
    $w = $test->ffi->tb_width();
    $h = $test->ffi->tb_height();
    $y = 0;
    while ($y < $h) {
        $test->ffi->tb_printf(0, $y++, 0, 0,
            "line=%d region_h=%d expanded=%s shrunk=%s",
            $y, $h, $expanded ? 'y' : 'n', $shrunk ? 'y' : 'n'
        );
    }
};

$test->ffi->tb_region(intdiv($term_h, 3));
$test->ffi->tb_init();
$fill_region();
$test->ffi->tb_present();

$expanded_h = intdiv($term_h, 2);
$test->ffi->tb_region($expanded_h);
$event = $test->ffi->new('struct tb_event');
$rv = $test->ffi->tb_peek_event(FFI::addr($event), 1000);
$expanded = $event->type === $test->defines['TB_EVENT_RESIZE']
    && $event->h === $expanded_h
    && $event->y === $term_h;
$fill_region();
$test->ffi->tb_present();

$shrunk_h = intdiv($term_h, 4);
$test->ffi->tb_region($shrunk_h);
$event = $test->ffi->new('struct tb_event');
$rv = $test->ffi->tb_peek_event(FFI::addr($event), 1000);
$shrunk = $event->type === $test->defines['TB_EVENT_RESIZE']
    && $event->h === $shrunk_h
    && $event->y === $term_h;
$fill_region();
$test->ffi->tb_present();

$test->screencap();
