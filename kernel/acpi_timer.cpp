//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#include <cstdint>
#include "acpi.hpp"
#include "asmfunc.hpp"

namespace acpi_timer {
    // FADTの構造
    struct __attribute__((packed)) Fadt {
        struct acpi::Sdth header;
        char reserved1[76 - sizeof(header)];
        uint32_t pm_tmr_blk;
        char reserved2[112 - 80];
        uint32_t flags;
    };

    uint32_t tmr_port;
    bool pm_timer_32;
    void init() {
        struct Fadt *fadt = (struct Fadt *)acpi::getSdt("FACP");
        tmr_port = fadt->pm_tmr_blk;
        pm_timer_32 = (fadt->flags >> 8) & 1;
    }

    uint32_t getTime() {
        return ioIn32(tmr_port);
    }
}  // namespace acpi_timer
