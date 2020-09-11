//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#include "interrupt.hpp"
#include "asmfunc.hpp"

namespace interrupt {
    GATE_DESCRIPTOR idt[256];

    // num番のデスクリプターを初期化する
    void initGateDescriptor(
        int num,
        void *handler,
        int16_t selector,
        unsigned char flags
    ) {
        u_int64_t handler_addr = (u_int64_t)handler;
        idt[num].offsetLow = (u_int16_t)(handler_addr & 0x000000000000FFFF);
        idt[num].offsetMiddle =
            (u_int16_t)(handler_addr & 0x00000000FFFF0000 >> 16);
        idt[num].offsetHigh = (u_int16_t)(handler_addr >> 32);
        idt[num].selector = selector;
        idt[num].flags = flags;
        idt[num].reserved1 = 0x00;
        idt[num].reserved2 = 0x00;
    }

    // IDTRを初期化する
    void idtr_init() {
        unsigned char flag =
            INTERRUPT_GATE | GATE_32BIT | GATE_DPL0 | SEGMENT_PRESENT;
        for (int i = 0; i < 256; i++) {
            initGateDescriptor(
                i,
                reinterpret_cast<void *>(default_handler),
                SEGMENT_SELECTOR,
                flag);
        }

        struct __attribute__((packed)) {
            u_int16_t           size;
            GATE_DESCRIPTOR*    base;
        } idtr;

        idtr.size = 256 * sizeof(GATE_DESCRIPTOR);
        idtr.base = idt;

        __asm__ (
            "lidt %0"
            :
            : "m"(idtr)
        );
    }

    // PICを初期化
    void pic_init() {
        // ICW1
        io_out32(PORT_MASTER_PIC_COMMAND, 0b00010001);
        io_out32(PORT_SLAVE_PIC_COMMAND,  0b00010001);

        // ICW2
        io_out32(PORT_MASTER_PIC_DATA, 0x20);
        io_out32(PORT_SLAVE_PIC_DATA, 0x28);

        // ICW3
        io_out32(PORT_MASTER_PIC_DATA, 0b00000100);
        io_out32(PORT_SLAVE_PIC_DATA,  0b010);

        // ICW4
        io_out32(PORT_MASTER_PIC_DATA, 0b00000001);
        io_out32(PORT_SLAVE_PIC_DATA,  0b00000001);

        // OCW1
        io_out32(PORT_MASTER_PIC_IMR, 0b11111111);
        io_out32(PORT_SLAVE_PIC_IMR,  0b11111111);
    }

    // picのマスク（無効化）を外す
    void pic_unmask(unsigned char intr_no) {
        // マスターPICが管理しているか
        bool master = false;
        if (intr_no < 40) master = true;

        if (master) {
            // マスター管理
            unsigned char ir_no = intr_no - 32;
            unsigned char ir_bit = 1U << ir_no;

            unsigned char mask = io_in32(PORT_MASTER_PIC_IMR);

            // 既にマスク解除済み
            if ((ir_bit & mask) != 0) return;

            // マスク解除
            io_out32(PORT_MASTER_PIC_IMR, mask & ~ir_bit);
        } else {
            // スレーブ管理
            unsigned char ir_no = intr_no - 40;
            unsigned char ir_bit = 1U << ir_no;

            unsigned char mask = io_in32(PORT_SLAVE_PIC_IMR);

            // 既にマスク解除済み
            if ((ir_bit & mask) != 0) return;

            // マスク解除
            io_out32(PORT_SLAVE_PIC_IMR, mask & ~ir_bit);
        }
    }

    // EOIを設定する
    void set_pic_eoi() {
        io_out32(PORT_MASTER_PIC_COMMAND, 0b00100000);
    }
}  // namespace interrupt
