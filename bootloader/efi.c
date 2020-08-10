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