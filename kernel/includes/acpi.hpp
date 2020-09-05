#pragma once

#include <cstring>

namespace acpi {
    //SDTHの構造
    struct __attribute__((packed)) SDTH {
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

    struct __attribute__((packed)) ACPI_ADDRESS {
        unsigned char address_space_id;
        unsigned char register_bit_width;
        unsigned char register_bit_offset;
        unsigned char _reserved;
        unsigned long long address;
    };

    //初期化処理
    extern void init(void *rsdp);

    extern struct SDTH *get_sdt(char *sig);
}