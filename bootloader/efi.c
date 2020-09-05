#include "efi.h"

EFI_SYSTEM_TABLE *ST;
EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;
EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SFSP;

/* SystemTableの初期化 */
void efi_init(EFI_SYSTEM_TABLE *SystemTable) {
    EFI_GUID GOP_GUID = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GUID SFSP_GUID = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

    ST = SystemTable;
    ST->BootServices->LocateProtocol(&GOP_GUID, NULL, (VOID **)&GOP);
    ST->BootServices->LocateProtocol(&SFSP_GUID, NULL, (void **)&SFSP);
}

/* ACPIテーブルを取得する */
void *getAcpiTable() {
    EFI_GUID efi_acpi_table_guid = EFI_ACPI_TABLE_GUID;

    for (int i = 0; i < ST->NumberOfTableEntries; i++) {
        EFI_GUID guid = ST->ConfigurationTable[i].VendorGuid;
        if (guid.Data1 == efi_acpi_table_guid.Data1 && guid.Data2 == efi_acpi_table_guid.Data2 && guid.Data3 == efi_acpi_table_guid.Data3) {
            unsigned char is_equal = 1;
            
            for (int j = 0; j < 8; j++) {
                if (guid.Data4[j] != efi_acpi_table_guid.Data4[j])
                    is_equal = 0;
            }
            
            //GUIDが一致したら返す
            if (is_equal == 1) return ST->ConfigurationTable[i].VendorTable;
        }
    }

    //一致しなかったらnullを返す
    return NULL;
}