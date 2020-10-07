//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#include "acpi.hpp"

namespace acpi {
    // RSDPの構造
    struct __attribute__((packed)) Rsdp {
        char Signature[8];
        unsigned char Checksum;
        char OEMID[6];
        unsigned char Revision;
        unsigned int RsdtAddress;
        unsigned int Length;
        uint64_t XsdtAddress;
        unsigned char Extended_Checksum;
        unsigned char Reserved[3];
    };

    // XSDTの構造
    struct __attribute__((packed)) Xsdt {
        struct Sdth Header;
        struct Sdth *Entry[0];
    };

    struct Xsdt *xsdt;
    int num_sdts;

    // 初期化処理
    void init(void *rsdp) {
        xsdt = (struct Xsdt *)((struct Rsdp *)rsdp)->XsdtAddress;
        num_sdts =
            (xsdt->Header.Length - sizeof(struct Sdth)) / sizeof(struct Sdth *);
    }

    // SDTを取得
    struct Sdth *getSdt(char *sig) {
        for (int i = 0; i < num_sdts; i++) {
            bool is_equal = true;
            for (int j = 0; j < 4; j++) {
                if (sig[j] != xsdt->Entry[i]->Signature[j]) is_equal = false;
            }
            if (is_equal) {
                return xsdt->Entry[i];
            }
        }

        return NULL;
    }
}  // namespace acpi
