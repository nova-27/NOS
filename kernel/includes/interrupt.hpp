/* Copyright (C) 2020 nova27. All rights reserved. */

#pragma once

#include <stdlib.h>

#define SEGMENT_SELECTOR 0x08

#define INTERRUPT_GATE   0b00000110
#define GATE_32BIT       0b00001000
#define GATE_DPL0        0b00000000
#define SEGMENT_PRESENT  0b10000000

#define PORT_MASTER_PIC_COMMAND     0x0020
#define PORT_MASTER_PIC_STATUS      0x0020
#define PORT_MASTER_PIC_DATA        0x0021
#define PORT_MASTER_PIC_IMR         0x0021

#define PORT_SLAVE_PIC_COMMAND      0x00A0
#define PORT_SLAVE_PIC_STATUS       0x00A0
#define PORT_SLAVE_PIC_DATA         0x00A1
#define PORT_SLAVE_PIC_IMR          0x00A1

namespace interrupt {
    typedef struct {
        u_int16_t   offsetLow;
        u_int16_t   selector;
        unsigned char   reserved1;
        unsigned char   flags;
        u_int16_t       offsetMiddle;
        unsigned int    offsetHigh;
        unsigned int    reserved2;
    } __attribute__((packed)) GATE_DESCRIPTOR;

    // num番のデスクリプターを初期化する
    extern void initGateDescriptor(int, void*, int16_t, unsigned char);
    // IDTRを初期化
    extern void idtr_init();
    // PICを初期化
    extern void pic_init();
    // PICのマスク（無効化）を外す
    extern void pic_unmask();
    // EOIを設定する
    extern void set_pic_eoi();
}  // namespace interrupt
