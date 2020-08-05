#include "efi.h"

#define EFI_SUCCESS               0x0
#define EFI_BUFFER_TOO_SMALL      0x8000000000000005

const CHAR16* GetMemoryTypeUnicode(EFI_MEMORY_TYPE type) {
  switch (type) {
    case EfiReservedMemoryType: return L"EfiReservedMemoryType";
    case EfiLoaderCode: return L"EfiLoaderCode";
    case EfiLoaderData: return L"EfiLoaderData";
    case EfiBootServicesCode: return L"EfiBootServicesCode";
    case EfiBootServicesData: return L"EfiBootServicesData";
    case EfiRuntimeServicesCode: return L"EfiRuntimeServicesCode";
    case EfiRuntimeServicesData: return L"EfiRuntimeServicesData";
    case EfiConventionalMemory: return L"EfiConventionalMemory";
    case EfiUnusableMemory: return L"EfiUnusableMemory";
    case EfiACPIReclaimMemory: return L"EfiACPIReclaimMemory";
    case EfiACPIMemoryNVS: return L"EfiACPIMemoryNVS";
    case EfiMemoryMappedIO: return L"EfiMemoryMappedIO";
    case EfiMemoryMappedIOPortSpace: return L"EfiMemoryMappedIOPortSpace";
    case EfiPalCode: return L"EfiPalCode";
    case EfiPersistentMemory: return L"EfiPersistentMemory";
    case EfiMaxMemoryType: return L"EfiMaxMemoryType";
    default: return L"InvalidMemoryType";
  }
}

/*struct MemoryMap {
  unsigned long long buffer_size;
  void* buffer;
  unsigned long long map_size;
  unsigned long long map_key;
  unsigned long long descriptor_size;
  uint32_t descriptor_version;
};*/

/* 整数を文字列にする */
static void intToChar16(int num, CHAR16 *result, int redix)
{
	/* 桁数を求める */
	int digit = 1;
	for (int i = num; i >= redix; i /= redix)
	{
		digit++;
	}

	/* 最後はnull文字 */
	result[digit] = '\0';

	/* 一の位から求めていく */
	int tmp = num;
	for (int i = digit; i > 0; i--)
	{
		int numOfPos = tmp % redix;
		if (numOfPos <= 9)
		{
			/*0から9の範囲*/
			result[i - 1] = numOfPos + L'0';
		}
		else
		{
			/*A(0xA)からの範囲*/
			result[i - 1] = numOfPos - 10 + L'A';
		}
		tmp /= redix;
	}
}

/* エントリポイント */
EFI_STATUS
efi_main(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable)
{
	/* 画面クリア */
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

	SystemTable->ConOut->OutputString(SystemTable->ConOut, L"**Nova27's Operating System Bootloader**\r\n");

	/*CHAR8 memmap_buf[4096 * 4];
  struct MemoryMap memmap = {sizeof(memmap_buf), memmap_buf, 0, 0, 0, 0};*/

	/* メモリマップの取得 */
	UINTN mapSize = 0, mapKey, descriptorSize;
	EFI_MEMORY_DESCRIPTOR *map = 0;
	EFI_STATUS result = -1;
	while(EFI_SUCCESS != (result = SystemTable->BootServices->GetMemoryMap(&mapSize, map, &mapKey, &descriptorSize, 1)))
    {
        if(result == EFI_BUFFER_TOO_SMALL)
        {
            mapSize += 2 * descriptorSize;
			SystemTable->BootServices->AllocatePool(EfiLoaderData, mapSize, (VOID **)&map);
        } else {
			SystemTable->ConOut->OutputString(SystemTable->ConOut, "Failed to get the memory map!");
		}
    }
	

	char *startOfMemoryMap = map;
    char *endOfMemoryMap = startOfMemoryMap + mapSize;

    char *offset = startOfMemoryMap;

    EFI_MEMORY_DESCRIPTOR *desc;

	int cnt = 0;

	/* メモリマップの表示 */
	SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\r\n<Debuging information>\r\n");
	SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Type, NumberOfPages, PhysicalStart, VirtualStart, Attribute\r\n");
    while(offset < endOfMemoryMap)
    {
        desc = (EFI_MEMORY_DESCRIPTOR *)offset;

		CHAR16 buf[11];

		SystemTable->ConOut->OutputString(SystemTable->ConOut, GetMemoryTypeUnicode(desc->Type));
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L", ");

		intToChar16(desc->NumberOfPages, buf, 10);
		SystemTable->ConOut->OutputString(SystemTable->ConOut, buf);
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"pages, 0x");

		intToChar16(desc->PhysicalStart, buf, 16);
		SystemTable->ConOut->OutputString(SystemTable->ConOut, buf);
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L", 0x");

		intToChar16(desc->VirtualStart, buf, 16);
		SystemTable->ConOut->OutputString(SystemTable->ConOut, buf);
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L", ");

		intToChar16(desc->Attribute, buf, 16);
		SystemTable->ConOut->OutputString(SystemTable->ConOut, buf);
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\r\n");

        offset += descriptorSize;

		if(cnt == 5) {
			break;
		}

		cnt++;
    }

	while (1);
}