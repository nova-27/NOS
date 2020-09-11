//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#pragma once

#include <stdlib.h>
#include <cstring>

namespace acpi {
    // SDTHの構造
    struct __attribute__((packed)) Sdth {
        char Signature[4];
        unsigned int Length;
        unsigned char Revision;
        unsigned char Checksum;
        char OEMID[6];
        char OEM_Table_ID[8];
        unsigned int OEM_Revision;
        unsigned int Creator_ID;
        unsigned int Creator_Revision;
    };

    struct __attribute__((packed)) AcpiAddress {
        unsigned char address_space_id;
        unsigned char register_bit_width;
        unsigned char register_bit_offset;
        unsigned char _reserved;
        u_int64_t address;
    };

    // 初期化処理
    extern void init(void *rsdp);

    // SDTを取得
    extern struct Sdth *get_sdt(char *sig);
}  // namespace acpi
