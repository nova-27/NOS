//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#pragma once

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
        uint16_t   offsetLow;
        uint16_t   selector;
        unsigned char   reserved1;
        unsigned char   flags;
        uint16_t       offsetMiddle;
        unsigned int    offsetHigh;
        unsigned int    reserved2;
    } __attribute__((packed)) GATE_DESCRIPTOR;

    // num番のデスクリプターを初期化する
    extern void initGateDescriptor(int, void*, int16_t, unsigned char);
    // IDTRを初期化
    extern void idtrInit();
    // PICを初期化
    extern void picInit();
    // PICのマスク（無効化）を外す
    extern void picUnmask(unsigned char intr_no);
    // EOIを設定する
    extern void setPicEoi();
}  // namespace interrupt
