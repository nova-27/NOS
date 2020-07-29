#include "efi.h"

EFI_STATUS
efi_main (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
) {
  SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello, world!");
  while (1);
}