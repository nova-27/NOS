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
	while(EFI_SUCCESS != (result = ST->BootServices->GetMemoryMap(&memInfo->mapSize, memInfo->map, &memInfo->mapKey, (UINTN *)&memInfo->descriptorSize, (UINT32 *)1)))
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

/* ファイルの大きさを取得する */
EFI_STATUS getFileSize(EFI_SYSTEM_TABLE *ST, EFI_FILE_PROTOCOL *file, UINT64 *fileSize)
{
	EFI_STATUS result = -1;
	void *buf = NULL;
	UINTN bufsize = 0;
	EFI_GUID fi_guid = EFI_FILE_INFO_ID;

	while(EFI_SUCCESS != (result = file->GetInfo(file, &fi_guid, &bufsize, &buf)))
    {
        if(result == EFI_BUFFER_TOO_SMALL)
        {
			//メモリマップ領域が足りなかったら、リトライ（失敗時にbufsizeは調整される）
			ST->BootServices->AllocatePool(EfiLoaderData, bufsize, &buf);
        } else {
			//それ以外のエラー
			return result;
		}
    }

	EFI_FILE_INFO *fi = buf;
	*fileSize = fi->FileSize;
	ST->BootServices->FreePool(buf);

	return EFI_SUCCESS;
}

/* エントリポイント */
EFI_STATUS
efi_main(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable)
{
	// 必要なデータを設定
	efi_init(SystemTable, ImageHandle);
	EFI_STATUS result = -1;

	// 画面クリア 
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	SystemTable->ConOut->OutputString(SystemTable->ConOut, (CHAR16 *)L"**Nova27's Operating System Bootloader**\r\n");

	// ドライブのrootを開く
	EFI_FILE_PROTOCOL *root;
	result = ESFSP->OpenVolume(ESFSP, &root);
	checkResult(SystemTable, result, (CHAR16 *)L"Failed to open drive!");

	// カーネルファイルを開く
	EFI_FILE_PROTOCOL *kernelFile;
	result = root->Open(root, &kernelFile, (CHAR16 *)L"kernel.elf", EFI_FILE_MODE_READ, 0);
	checkResult(SystemTable, result, (CHAR16 *)L"Failed to open kernel file!");

	// カーネルファイルを読み込む
	UINT64 kernelFileSize;
	void* kernelFileBuf;
	getFileSize(SystemTable, kernelFile, &kernelFileSize);
	checkResult(SystemTable, result, (CHAR16 *)L"Failed to get the size of the kernel file!");
	SystemTable->BootServices->AllocatePool(EfiLoaderData, kernelFileSize, &kernelFileBuf);
	kernelFile->Read(kernelFile, &kernelFileSize, (void *)kernelFileBuf);
	checkResult(SystemTable, result, (CHAR16 *)L"Failed to read kernel file!");

	// ファイル等を閉じる
	kernelFile->Close(kernelFile);
	root->Close(root);

	// メモリマップの取得
	struct MemoryInfo memInfo = {0, NULL, 0, 0};
	result = getMemoryMap(SystemTable, &memInfo);
	checkResult(SystemTable, result, (CHAR16 *)L"Failed to get the memory map!");

	//BootServicesを抜ける
	result = SystemTable->BootServices->ExitBootServices(ImageHandle, memInfo.mapKey);
  	if (result == EFI_INVALID_PARAMETER) {
		//失敗したら再度メモリマップを取得し、ExitBootServicesを再実行
    	result = getMemoryMap(SystemTable, &memInfo);
    	checkResult(SystemTable, result, (CHAR16 *)L"Failed to get the memory map!");

    	result = SystemTable->BootServices->ExitBootServices(ImageHandle, memInfo.mapKey);
		checkResult(SystemTable, result, (CHAR16 *)L"Could not exit boot service!");
  	}

	return EFI_SUCCESS;
}