//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#include <stdint.h>
#include "acpi_timer.hpp"
#include "interrupt.hpp"
#include "asmfunc.hpp"
#include "serial_port.hpp"

namespace apic_timer {
    uint32_t *divide_config = reinterpret_cast<uint32_t*>(0xfee003e0);
    uint32_t *initial_count = reinterpret_cast<uint32_t*>(0xfee00380);
    uint32_t *current_count = reinterpret_cast<uint32_t*>(0xfee00390);
    uint32_t *lvt_timer = reinterpret_cast<uint32_t*>(0xfee00320);

    void Init() {
        // 除算値を16に設定
        *divide_config = 0x3;

        // 計測
        *initial_count = static_cast<uint32_t>(0xFFFFFFFF);
        u_int32_t target_val = acpi_timer::GetTime() + 0.01 * 3579545;
        while (acpi_timer::GetTime() < target_val) {}
        uint32_t ticksIn10ms = 0xFFFFFFFF - *current_count;
        *initial_count = 0;

        // 10ミリ秒ごとにIRQ0に割り込み信号を送るように設定
        *lvt_timer = 32 | 0x20000;
        *divide_config = 0x3;
        *initial_count = ticksIn10ms * 1000;
    }
}  // namespace apic_timer
