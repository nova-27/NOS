//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#include "hpet.hpp"
using namespace acpi;

#define GCIDR_ADDR (reg_base)
#define GCIDR (*(u_int64_t *)GCIDR_ADDR)

#define GCR_ADDR (reg_base + 0x10)
#define GCR (*(u_int64_t *)GCR_ADDR)

#define MCR_ADDR (reg_base + 0xf0)
#define MCR (*(u_int64_t *)MCR_ADDR)

namespace hpet {
    struct __attribute__((packed)) HpetTable {
        struct Sdth header;
        unsigned int event_timer_block_id;
        struct AcpiAddress base_address;
        unsigned char hpet_number;
        u_int16_t minimum_tick;
        unsigned char flags;
    };

    union gcr {
        u_int64_t raw;
        struct __attribute__((packed)) {
            u_int64_t enable_cnf:1;
            u_int64_t leg_rt_cnf:1;
            u_int64_t _reserved:62;
        };
    };

    union gcidr {
        u_int64_t raw;
        struct __attribute__((packed)) {
            u_int64_t rev_id:8;
            u_int64_t num_tim_cap:5;
            u_int64_t count_size_cap:1;
            u_int64_t _reserved:1;
            u_int64_t leg_rt_cap:1;
            u_int64_t vendor_id:16;
            u_int64_t counter_clk_period:32;
        };
    };

    u_int64_t reg_base;
    void init() {
        struct HpetTable *hpet_table = (struct HpetTable *)get_sdt("HPET");
        reg_base = hpet_table->base_address.address;

        union gcr gcr;
        gcr.raw = (*reinterpret_cast<u_int64_t *>(reg_base + 0x10));
        gcr.enable_cnf = 0;
        (*reinterpret_cast<u_int64_t *>(reg_base + 0x10)) = gcr.raw;
    }

    void sleep(u_int64_t us) {
        u_int64_t mc_now = MCR;

        // us マイクロ秒後の main counter のカウント値を算出
        u_int64_t fs = us * 1000000000;
        union gcidr gcidr;
        gcidr.raw = GCIDR;
        u_int64_t mc_duration = fs / gcidr.counter_clk_period;
        u_int64_t mc_after = mc_now + mc_duration;
        // HPET が無効であれば有効化する
        union gcr gcr;
        gcr.raw = GCR;
        unsigned char to_disable = 0;
        if (!gcr.enable_cnf) {
            gcr.enable_cnf = 1;
            GCR = gcr.raw;
            // sleep() を抜ける際に元に戻す (disable する)
            to_disable = 1;
        }
        // us マイクロ秒の経過を待つ
        while (MCR < mc_after) {}
        // 元々無効であった場合は無効に戻しておく
        if (to_disable) {
            gcr.raw = GCR;
            gcr.enable_cnf = 0;
            GCR = gcr.raw;
        }
    }

    int getTime() {
        return MCR;
    }
}  // namespace hpet
