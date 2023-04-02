#!/usr/bin/env php
<?php

$termbox_h = <<<'EOD'
    struct tb_event {
        uint8_t type;
        uint8_t mod;
        uint16_t key;
        uint32_t ch;
        int32_t w;
        int32_t h;
        int32_t x;
        int32_t y;
    };
    int tb_init();
    int tb_shutdown();
    int tb_present();
    int tb_poll_event(struct tb_event *event);
    int tb_printf(int x, int y, uint32_t fg, uint32_t bg, const char *fmt, ...);
EOD;

$termbox = FFI::cdef($termbox_h, '../libtermbox2.so');

$ev = $termbox->new('struct tb_event');

$termbox->tb_init();

$y = 0;
$termbox->tb_printf(0, $y++, 0x02 | 0x0100, 0x00, 'hello from php');
$termbox->tb_printf(0, $y++, 0x03, 0x00, 'press any key');
$termbox->tb_present();
$termbox->tb_poll_event(FFI::addr($ev));

$termbox->tb_printf(0, $y++, 0x04, 0x00,
    'event: type=%d mod=%d key=%d ch=%d w=%d h=%d x=%d y=%d',
    $ev->type,
    $ev->mod,
    $ev->key,
    $ev->ch,
    $ev->w,
    $ev->h,
    $ev->x,
    $ev->y);
$termbox->tb_present();

$termbox->tb_printf(0, $y++, 0x05, 0x00, 'press any key to quit');
$termbox->tb_present();
$termbox->tb_poll_event(FFI::addr($ev));

$termbox->tb_shutdown();
