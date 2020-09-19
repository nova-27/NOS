//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#include "acpi.hpp"
#include "asmfunc.hpp"

namespace acpi_timer {
    // FADTの構造
    struct __attribute__((packed)) Fadt {
        struct acpi::Sdth header;
        char reserved1[76 - sizeof(header)];
        u_int32_t pm_tmr_blk;
        char reserved2[112 - 80];
        u_int32_t flags;
    };

    u_int32_t tmr_port;
    bool pm_timer_32;
    void Init() {
        struct Fadt *fadt = (struct Fadt *)acpi::get_sdt("FACP");
        tmr_port = fadt->pm_tmr_blk;
        pm_timer_32 = (fadt->flags >> 8) & 1;
    }

    u_int32_t GetTime() {
        return io_in32(tmr_port);
    }
}  // namespace acpi_timer
