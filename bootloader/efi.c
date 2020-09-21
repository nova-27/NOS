//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#include "includes/efi.h"

EFI_SYSTEM_TABLE *st;
EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *sfsp;

/* SystemTableの初期化 */
void efiInit(EFI_SYSTEM_TABLE *system_table) {
    EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GUID sfsp_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

    st = system_table;
    st->BootServices->LocateProtocol(&gop_guid, NULL, (VOID **)&gop);
    st->BootServices->LocateProtocol(&sfsp_guid, NULL, (void **)&sfsp);
}

/* ACPIテーブルを取得する */
void *getAcpiTable() {
    EFI_GUID efi_acpi_table_guid = EFI_ACPI_TABLE_GUID;

    for (UINTN i = 0; i < st->NumberOfTableEntries; i++) {
        EFI_GUID guid = st->ConfigurationTable[i].VendorGuid;
        if (guid.Data1 == efi_acpi_table_guid.Data1 &&
            guid.Data2 == efi_acpi_table_guid.Data2 &&
            guid.Data3 == efi_acpi_table_guid.Data3) {
            unsigned char is_equal = 1;

            for (int j = 0; j < 8; j++) {
                if (guid.Data4[j] != efi_acpi_table_guid.Data4[j])
                    is_equal = 0;
            }

            // GUIDが一致したら返す
            if (is_equal == 1) return st->ConfigurationTable[i].VendorTable;
        }
    }

    // 一致しなかったらnullを返す
    return NULL;
}
