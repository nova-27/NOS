#include "efi.h"
#include "utils.h"

/* メモリに関する情報 */
struct MemoryInfo
{
	UINTN mapSize;
	EFI_MEMORY_DESCRIPTOR *map;
	UINTN descriptorSize;
	UINTN mapKey;
};

/* メモリマップを取得する */
EFI_STATUS getMemoryMap(EFI_SYSTEM_TABLE *ST, struct MemoryInfo *memInfo) {
	EFI_STATUS result = -1;
	while(EFI_SUCCESS != (result = ST->BootServices->GetMemoryMap(&memInfo->mapSize, memInfo->map, &memInfo->mapKey, (UINT32 *)&memInfo->descriptorSize, 1)))
    {
        if(result == EFI_BUFFER_TOO_SMALL)
        {
			//メモリマップ領域が足りなかったら、リトライ（失敗時にmapSizeは調整される）
			ST->BootServices->AllocatePool(EfiLoaderData, memInfo->mapSize, (VOID **)&memInfo->map);
        } else {
			//それ以外のエラー
			return result;
		}
    }

	return EFI_SUCCESS;
}

/* エントリポイント */
EFI_STATUS
efi_main(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable)
{
	// 必要なデータを設定
	efi_init(SystemTable);
	EFI_STATUS result = -1;

	// 画面クリア 
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	SystemTable->ConOut->OutputString(SystemTable->ConOut, L"**Nova27's Operating System Bootloader**\r\n");

	// メモリマップの取得
	struct MemoryInfo memInfo = {0, NULL, 0, 0};
	if(EFI_SUCCESS != getMemoryMap(SystemTable, &memInfo)) {
		//失敗したら停止
		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Failed to get the memory map!");
		halt();
	}

	//BootServicesを抜ける
	result = SystemTable->BootServices->ExitBootServices(ImageHandle, memInfo.mapKey);
  	if (result == EFI_INVALID_PARAMETER) {
		//失敗したら再度メモリマップを取得し、ExitBootServicesを再実行
    	result = getMemoryMap(SystemTable, &memInfo);
    	if (EFI_SUCCESS != result) {
      		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Failed to get the memory map!");
      		halt();
    	}
    	result = SystemTable->BootServices->ExitBootServices(ImageHandle, memInfo.mapKey);
    	if (EFI_SUCCESS != result) {
      		SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Could not exit boot service");
      		halt();
    	}
  	}

	/*char *startOfMemoryMap = map;
    char *endOfMemoryMap = startOfMemoryMap + mapSize;

    char *offset = startOfMemoryMap;

    EFI_MEMORY_DESCRIPTOR *desc;

	int cnt = 0;

	/* メモリマップの表示 */
	/*SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\r\n<Debuging information>\r\n");
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
    }*/

	halt();
}