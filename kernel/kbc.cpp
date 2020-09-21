//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#include <cstdint>
#include "asmfunc.hpp"
#include "interrupt.hpp"
#include "kbc.hpp"
#include "serial_port.hpp"

#define KBC_DATA_ADDR          0x0060
#define KBC_DATA_BIT_IS_BRAKE  0x80
#define KBC_STATUS_ADDR        0x0064
#define KBC_STATUS_BIT_OBF     0x01
#define KBC_INTR_NO            33

const char keymap[] = {
    0x00, ASCII_ESC, '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '^', ASCII_BS, ASCII_HT,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '@', '[', '\n', 0x00, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    ':', 0x00, 0x00, ']', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0x00, '*',
    0x00, ' ', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, '7',
    '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, '_', 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, '\\', 0x00, 0x00
};

extern "C" void kbcInterrupt() {
    // ステータスレジスタのOBFがセットされていなければreturn
    if (!(ioIn8(KBC_STATUS_ADDR) & KBC_STATUS_BIT_OBF)) return;
        //goto kbc_exit;

    // make状態でなければreturn
    unsigned char keycode = ioIn8(KBC_DATA_ADDR);
    if (keycode & KBC_DATA_BIT_IS_BRAKE) return;
        //goto kbc_exit;

    // エコーバック処理
    char c = keymap[keycode];
    if (('a' <= c) && (c <= 'z'))
        c = c - 'a' + 'A';

    while (ioIn8(COM1 + 5) & 0x20 == 0) {}
    ioOut8(COM1, c);

kbc_exit:
    // PICへ割り込み処理終了を通知(EOI)
    interrupt::setPicEoi();
}

void kbcInit() {
    unsigned char flag =
            INTERRUPT_GATE | GATE_32BIT | GATE_DPL0 | SEGMENT_PRESENT;
    interrupt::initGateDescriptor(KBC_INTR_NO,
    reinterpret_cast<void *>(kbcHandler),
        SEGMENT_SELECTOR,
        flag);
    interrupt::picUnmask(KBC_INTR_NO);
}

unsigned char get_kbc_data() {
    /* ステータスレジスタの OBF がセットされるまで待つ */
    while (!(ioIn8(KBC_STATUS_ADDR) & KBC_STATUS_BIT_OBF)) {}
    return ioIn8(KBC_DATA_ADDR);
}

unsigned char get_keycode() {
    unsigned char keycode;
    /* make 状態 (brake ビットがセットされていない状態) まで待つ */
    while ((keycode = get_kbc_data()) & KBC_DATA_BIT_IS_BRAKE) {}

    return keycode;
}

char getc() {
    return keymap[get_keycode()];
}