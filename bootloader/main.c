#include "efi.h"
#include "utils.h"
#include "elf.h"
#include <math.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

//EFI_SYSTEM_TABLE *ST;

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
	EFI_FILE_INFO *Buffer = NULL;
	UINTN BufferSize = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 12;
	EFI_GUID fi_guid = EFI_FILE_INFO_ID;

	if(EFI_SUCCESS != (result = file->GetInfo(file, &fi_guid, &BufferSize, Buffer))) return result;

	*fileSize = Buffer->FileSize;

	return EFI_SUCCESS;
}

//TODO

void getElfStartLastAddr(EFI_SYSTEM_TABLE *ST, Elf64_Ehdr *elf_header, UINT64 *start_addr, UINT64 *last_addr) {
	Elf64_Phdr *program_header_table = (Elf64_Phdr *)((UINT64)elf_header + elf_header->e_phoff);
	*start_addr = 18446744073709551615;
	*last_addr = 0;
	
    for (UINTN i = 0; i < elf_header->e_phnum; i++) {
        Elf64_Phdr program_header = program_header_table[i];
        if (program_header.p_type != PT_LOAD) continue;

		*start_addr = MIN(*start_addr, program_header.p_vaddr);
        *last_addr = MAX(*last_addr, program_header.p_vaddr + program_header.p_memsz);
	}
}

void memCopyElf(EFI_SYSTEM_TABLE *ST, Elf64_Ehdr *elf_header, void *tmpBufAddr) {
	Elf64_Phdr *program_header_table = (Elf64_Phdr *)((UINT64)elf_header + elf_header->e_phoff);
	for (UINTN i = 0; i < elf_header->e_phnum; i++) {
        Elf64_Phdr program_header = program_header_table[i];
        if (program_header.p_type != PT_LOAD) continue;

		for(int i = 0; i < program_header.p_filesz; i++) {
			char *p = program_header.p_vaddr + i;
			*p = *(char *)(tmpBufAddr + program_header.p_offset + i);
		}
        //ST->BootServices->CopyMem((void *)program_header.p_vaddr, tmpBufAddr + program_header.p_offset, program_header.p_filesz);
		for(int i = 0; i < program_header.p_memsz - program_header.p_filesz; i++) {
			char *p = program_header.p_vaddr + program_header.p_filesz + i;
			*p = 0x00;
		}
    	//ST->BootServices->SetMem((void *)program_header.p_vaddr + program_header.p_filesz, program_header.p_memsz - program_header.p_filesz,  0);
	}
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
	SystemTable->ConOut->OutputString(SystemTable->ConOut, (CHAR16 *)L"Opening root directory...\r\n");
	EFI_FILE_PROTOCOL *root;
	result = ESFSP->OpenVolume(ESFSP, &root);
	checkResult(SystemTable, result, (CHAR16 *)L"Failed to open drive!");

	// カーネルファイルを開く
	SystemTable->ConOut->OutputString(SystemTable->ConOut, (CHAR16 *)L"Opening kernel file...\r\n");
	EFI_FILE_PROTOCOL *kernelFile;
	result = root->Open(root, &kernelFile, (CHAR16 *)L"kernel.elf", EFI_FILE_MODE_READ, 0);
	checkResult(SystemTable, result, (CHAR16 *)L"Failed to open kernel file!");

	// カーネルファイルを読み込む
	SystemTable->ConOut->OutputString(SystemTable->ConOut, (CHAR16 *)L"Getting kernel size...\r\n");
	UINT64 kernelFileSize;
	void* kernelFileBuf;
	getFileSize(SystemTable, kernelFile, &kernelFileSize);
	checkResult(SystemTable, result, (CHAR16 *)L"Failed to get the size of the kernel file!");

	SystemTable->ConOut->OutputString(SystemTable->ConOut, (CHAR16 *)L"Reading kernel file...\r\n");
	SystemTable->BootServices->AllocatePool(EfiLoaderData, kernelFileSize, &kernelFileBuf);
	kernelFile->Read(kernelFile, &kernelFileSize, (void *)kernelFileBuf);
	checkResult(SystemTable, result, (CHAR16 *)L"Failed to read kernel file!");

	//
	Elf64_Ehdr *kernel_header = (Elf64_Ehdr *)kernelFileBuf;
	UINT64 kernelStartAddr, kernelLastAddr;
	getElfStartLastAddr(SystemTable, kernel_header, &kernelStartAddr, &kernelLastAddr);

	int requiredPages = (kernelLastAddr - kernelStartAddr + 0x3fff) / 0x1000 / 0x4 /*4KB*/;
	SystemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, requiredPages, (void **)&kernelStartAddr);
	memCopyElf(SystemTable, kernel_header, kernelFileBuf);

	UINT64 entry_addr = kernel_header->e_entry;
	struct fb fb = {GOP->Mode->FrameBufferBase, GOP->Mode->FrameBufferSize, GOP->Mode->Info->HorizontalResolution, GOP->Mode->Info->VerticalResolution};

	// ファイル等を閉じる
	SystemTable->ConOut->OutputString(SystemTable->ConOut, (CHAR16 *)L"Closing files...\r\n");
	kernelFile->Close(kernelFile);
	root->Close(root);

	// メモリマップの取得
	SystemTable->ConOut->OutputString(SystemTable->ConOut, (CHAR16 *)L"Getting the memory map...\r\n");
	struct MemoryInfo memInfo = {0, NULL, 0, 0};
	result = getMemoryMap(SystemTable, &memInfo);
	checkResult(SystemTable, result, (CHAR16 *)L"Failed to get the memory map!");

	//BootServicesを抜ける
	SystemTable->ConOut->OutputString(SystemTable->ConOut, (CHAR16 *)L"Exiting Boot Services...\r\n");
	result = SystemTable->BootServices->ExitBootServices(ImageHandle, memInfo.mapKey);
  	if (result == EFI_INVALID_PARAMETER) {
		//失敗したら再度メモリマップを取得し、ExitBootServicesを再実行
    	result = getMemoryMap(SystemTable, &memInfo);
    	checkResult(SystemTable, result, (CHAR16 *)L"Failed to get the memory map!");

    	result = SystemTable->BootServices->ExitBootServices(ImageHandle, memInfo.mapKey);
		checkResult(SystemTable, result, (CHAR16 *)L"Could not exit boot service!");
  	}

	EFI_PHYSICAL_ADDRESS fb_addr = &fb;
	__asm__ (
		 "mov  %0, %%rdi\n"
		 "jmp  *%1\n"
		 ::
		 "m"(fb_addr),
		 "m"(entry_addr));

	return EFI_SUCCESS;
}