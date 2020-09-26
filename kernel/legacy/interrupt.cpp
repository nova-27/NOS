//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#include <cstdint>
#include "interrupt.hpp"
#include "asmfunc.hpp"

namespace interrupt {
    GateDescriptor idt[256];

    // num番のデスクリプターを初期化する
    void initGateDescriptor(
        int num,
        void *handler,
        unsigned char flags
    ) {
        uint64_t handler_addr = (uint64_t)handler;
        idt[num].offsetLow = (handler_addr);
        idt[num].offsetMiddle =
            (handler_addr & 0x00000000FFFF0000) >> 16;
        idt[num].offsetHigh = (handler_addr >> 32);
        idt[num].selector = SEGMENT_SELECTOR;
        idt[num].flags = flags;
        idt[num].present = 1;
    }

    // IDTRを初期化する
    void idtrInit() {
        unsigned char flag =
            INTERRUPT_GATE | GATE_32BIT | GATE_DPL0;
        for (int i = 0; i < 256; i++) {
            initGateDescriptor(
                i,
                reinterpret_cast<void *>(defaultHandler),
                flag);
        }

        struct __attribute__((packed)) {
            uint16_t           size;
            GateDescriptor*    base;
        } idtr;

        idtr.size = sizeof(idt) - 1;
        idtr.base = idt;

        __asm__ (
            "lidt %0"
            :
            : "m"(idtr)
        );
    }

    // PICを初期化
    void picInit() {
        // マスターPIC
        ioOut8(PORT_MASTER_PIC_COMMAND, 0b00010001);
        ioOut8(PORT_MASTER_PIC_DATA, 32);
        ioOut8(PORT_MASTER_PIC_DATA, 0b00000100);
        ioOut8(PORT_MASTER_PIC_DATA, 0b00000001);

        // スレーブPIC
        ioOut8(PORT_SLAVE_PIC_COMMAND,  0b00010001);
        ioOut8(PORT_SLAVE_PIC_DATA, 40);
        ioOut8(PORT_SLAVE_PIC_DATA,  0b010);
        ioOut8(PORT_SLAVE_PIC_DATA,  0b00000001);

        // OCW1
        ioOut8(PORT_MASTER_PIC_IMR, 0b11111111);
        ioOut8(PORT_SLAVE_PIC_IMR,  0b11111111);
    }

    // picのマスク（無効化）を外す
    void picUnmask(unsigned char intr_no) {
        // マスターPICが管理しているか
        bool master = false;
        if (intr_no < 40) master = true;

        if (master) {
            // マスター管理
            unsigned char ir_no = intr_no - 32;
            unsigned char ir_bit = 1U << ir_no;

            unsigned char mask = ioIn8(PORT_MASTER_PIC_IMR);

            // 既にマスク解除済み
            if ((ir_bit & mask) == 0) return;

            // マスク解除
            ioOut8(PORT_MASTER_PIC_IMR, mask & ~ir_bit);
        } else {
            // スレーブ管理
            unsigned char ir_no = intr_no - 40;
            unsigned char ir_bit = 1U << ir_no;

            unsigned char mask = ioIn8(PORT_SLAVE_PIC_IMR);

            // 既にマスク解除済み
            if ((ir_bit & mask) == 0) return;

            // マスク解除
            ioOut8(PORT_SLAVE_PIC_IMR, mask & ~ir_bit);
        }
    }

    // EOIを設定する
    void setPicEoi() {
        ioOut8(PORT_MASTER_PIC_COMMAND, 0b00100000);
    }
}  // namespace interrupt
