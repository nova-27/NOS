//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#include <cstdint>
#include "acpi_timer.hpp"
#include "interrupt.hpp"
#include "asmfunc.hpp"
#include "serial_port.hpp"

namespace apic_timer {
    uint32_t *divide_config = reinterpret_cast<uint32_t*>(0xfee003e0);
    uint32_t *initial_count = reinterpret_cast<uint32_t*>(0xfee00380);
    uint32_t *current_count = reinterpret_cast<uint32_t*>(0xfee00390);
    uint32_t *lvt_timer = reinterpret_cast<uint32_t*>(0xfee00320);
    uint32_t *end_of_interrupt = reinterpret_cast<uint32_t*>(0xfee000b0);

    void init() {
        // 除算値を16に設定
        *divide_config = 0x3;

        // 計測
        *initial_count = static_cast<uint32_t>(0xFFFFFFFF);
        uint32_t target_val = acpi_timer::getTime() + 0.01 * 3579545;
        while (acpi_timer::getTime() < target_val) {}
        uint32_t ticksIn10ms = 0xFFFFFFFF - *current_count;
        *initial_count = 0;

        // ハンドラを登録する
        unsigned char flag =
            INTERRUPT_GATE | GATE_32BIT | GATE_DPL0;
        interrupt::initGateDescriptor(
            48,
            reinterpret_cast<void *>(timerHandler),
            flag);

        // 10ミリ秒ごとにIRQ0に割り込み信号を送るように設定
        *lvt_timer = 48 | 0x20000;
        *divide_config = 0x3;
        *initial_count = ticksIn10ms * 100;
    }

    // 割り込みで呼ばれる
    extern "C" void timerInterrupt() {
        while ((ioIn8(COM1 + 5) & 0x20) == 0) {}
        ioOut8(COM1, 'A');

        *end_of_interrupt = 0;
    }
}  // namespace apic_timer
