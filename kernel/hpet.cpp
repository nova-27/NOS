#include "hpet.hpp"
using namespace acpi;

#define GCIDR_ADDR (reg_base)
#define GCIDR (*(unsigned long long *)GCIDR_ADDR)

#define GCR_ADDR (reg_base + 0x10)
#define GCR (*(unsigned long long *)GCR_ADDR)

#define MCR_ADDR (reg_base + 0xf0)
#define MCR (*(unsigned long long *)MCR_ADDR)

namespace hpet{
    struct __attribute__((packed)) HPET_TABLE {
        struct SDTH header;
        unsigned int event_timer_block_id;
        struct ACPI_ADDRESS base_address;
        unsigned char hpet_number;
        unsigned short minimum_tick;
        unsigned char flags;
    };

    union gcr {
        unsigned long long raw;
        struct __attribute__((packed)) {
            unsigned long long enable_cnf:1;
            unsigned long long leg_rt_cnf:1;
            unsigned long long _reserved:62;
        };
    };

    union gcidr {
        unsigned long long raw;
        struct __attribute__((packed)) {
            unsigned long long rev_id:8;
            unsigned long long num_tim_cap:5;
            unsigned long long count_size_cap:1;
            unsigned long long _reserved:1;
            unsigned long long leg_rt_cap:1;
            unsigned long long vendor_id:16;
            unsigned long long counter_clk_period:32;
        };
    };

    unsigned long long reg_base;
    void init() {
        struct HPET_TABLE *hpet_table = (struct HPET_TABLE *)get_sdt("HPET");
        reg_base = hpet_table->base_address.address;

        union gcr gcr;
        gcr.raw = (*(unsigned long long *)(reg_base + 0x10));
        gcr.enable_cnf = 0;
        (*(unsigned long long *)(reg_base + 0x10)) = gcr.raw;
    }

    void sleep(unsigned long long us) {
        unsigned long long mc_now = MCR;

        /* us マイクロ秒後の main counter のカウント値を算出 */
        unsigned long long fs = us * 1000000000;
        union gcidr gcidr;
        gcidr.raw = GCIDR;
        unsigned long long mc_duration = fs / gcidr.counter_clk_period;
        unsigned long long mc_after = mc_now + mc_duration;
        /* HPET が無効であれば有効化する */
        union gcr gcr;
        gcr.raw = GCR;
        unsigned char to_disable = 0;
        if (!gcr.enable_cnf) {
            gcr.enable_cnf = 1;
            GCR = gcr.raw;
            /* sleep() を抜ける際に元に戻す (disable する) */
            to_disable = 1;
        }
        /* us マイクロ秒の経過を待つ */
        while (MCR < mc_after);
        /* 元々無効であった場合は無効に戻しておく */
        if (to_disable) {
            gcr.raw = GCR;
            gcr.enable_cnf = 0;
            GCR = gcr.raw;
        }
    }
}