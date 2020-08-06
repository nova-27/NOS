#include "efi.h"

EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;

/* SystemTableの初期化 */
void efi_init(EFI_SYSTEM_TABLE *ST) {
    EFI_GUID GOP_GUID = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;

    ST->BootServices->LocateProtocol(&GOP_GUID, NULL, (VOID **)&GOP);
}