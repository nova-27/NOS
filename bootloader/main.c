/* Copyright (C) 2020 nova27. All rights reserved. */

#include "includes/efi.h"
#include "includes/utils.h"
#include "includes/elf.h"
#include "includes/common.h"

/* メモリマップを取得する */
EFI_STATUS getMemoryMap(struct MemoryInfo *mem_info) {
    EFI_STATUS result = -1;

    while (1) {
        result = st->BootServices->GetMemoryMap(
            &mem_info->mapSize,
            mem_info->map,
            &mem_info->mapKey,
            (UINTN *)&mem_info->descriptorSize,
            (UINT32 *)1);
        if (EFI_SUCCESS == result) break;  // 成功したら抜ける

        if (result == EFI_BUFFER_TOO_SMALL) {
            // メモリマップ領域が足りなかったら調整
            st->BootServices->AllocatePool(
                EfiLoaderData,
                mem_info->mapSize,
                (VOID **)&mem_info->map);
        } else {
            // それ以外のエラー
            return result;
        }
    }

    return EFI_SUCCESS;
}

/* ファイルの大きさを取得する */
EFI_STATUS getFileSize(EFI_FILE_PROTOCOL *file, UINT64 *file_size) {
    EFI_STATUS result = -1;
    EFI_FILE_INFO *buffer = NULL;
    UINTN buffer_size = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 12;
    EFI_GUID fi_guid = EFI_FILE_INFO_ID;

    result = file->GetInfo(file, &fi_guid, &buffer_size, buffer);
    if (EFI_SUCCESS != result) return result;

    *file_size = buffer->FileSize;

    return EFI_SUCCESS;
}

/* Elfを設置する最初と最後のアドレスを取得する */
void getElfStartLastAddr(
    Elf64_Ehdr *elf_header,
    UINT64 *start_addr,
    UINT64 *last_addr
) {
    Elf64_Phdr *program_header_table =
        (Elf64_Phdr *)((UINT64)elf_header + elf_header->e_phoff);
    *start_addr = 18446744073709551615ull;
    *last_addr = 0;

    for (UINTN i = 0; i < elf_header->e_phnum; i++) {
        Elf64_Phdr program_header = program_header_table[i];
        // PT_LOAD以外はメモリに読み込まなくていい
        if (program_header.p_type != PT_LOAD) continue;

        *start_addr =
            MIN(*start_addr, program_header.p_vaddr);
        *last_addr =
            MAX(*last_addr, program_header.p_vaddr + program_header.p_memsz);
    }
}

/* Elfをメモリにコピーする */
void memCopyElf(Elf64_Ehdr *elf_header, void *tmp_buf_addr) {
    Elf64_Phdr *program_header_table =
        (Elf64_Phdr *)((UINT64)elf_header + elf_header->e_phoff);

    for (UINTN i = 0; i < elf_header->e_phnum; i++) {
        Elf64_Phdr program_header = program_header_table[i];
        // PT_LOAD以外は読み込まなくていい
        if (program_header.p_type != PT_LOAD) continue;

        // ファイルバッファからコピー
        for (int i = 0; i < (int)program_header.p_filesz; i++) {
            char *p = (char *)program_header.p_vaddr + i;
            *p = *(char *)(tmp_buf_addr + program_header.p_offset + i);
        }
        // ファイルバッファに存在しない分は0x00で埋める
        int zerofill_area = program_header.p_memsz - program_header.p_filesz;
        for (int i = 0; i < zerofill_area; i++) {
            char *p =
                (char *)program_header.p_vaddr + program_header.p_filesz + i;
            *p = 0x00;
        }
    }
}

/* エントリポイント */
EFI_STATUS
efiMain(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
) {
    // 必要なデータを設定
    efiInit(SystemTable);
    EFI_STATUS result = -1;
    CHAR16 str_int_buf[20];

    // 画面クリア
    st->ConOut->ClearScreen(st->ConOut);
    st->ConOut->OutputString(
        st->ConOut,
        (CHAR16 *)L"**Nova27's Operating System Bootloader**\r\n");

    // ドライブのrootを開く
    st->ConOut->OutputString(
        st->ConOut,
        (CHAR16 *)L"Opening root directory...\r\n");
    EFI_FILE_PROTOCOL *root;
    result = sfsp->OpenVolume(sfsp, &root);
    checkResult(result, (CHAR16 *)L"Failed to open drive!");

    // カーネルファイルを開く
    st->ConOut->OutputString(
        st->ConOut,
        (CHAR16 *)L"Opening kernel file...\r\n");
    EFI_FILE_PROTOCOL *kernel_file;
    result = root->Open(
        root,
        &kernel_file,
        (CHAR16 *)L"kernel.elf",
        EFI_FILE_MODE_READ,
        0);
    checkResult(result, (CHAR16 *)L"Failed to open kernel file!");

    // カーネルファイルのサイズを取得
    st->ConOut->OutputString(
        st->ConOut,
        (CHAR16 *)L"Getting kernel size...\r\n");
    UINT64 kernel_file_size;
    void* kernel_file_buf;
    getFileSize(kernel_file, &kernel_file_size);
    checkResult(
        result,
        (CHAR16 *)L"Failed to get the size of the kernel file!");

    intToChar16(kernel_file_size, str_int_buf, 10);
    st->ConOut->OutputString(
        st->ConOut,
        (CHAR16 *)L"The size of the kernel file : ");
    st->ConOut->OutputString(st->ConOut, str_int_buf);
    st->ConOut->OutputString(st->ConOut, (CHAR16 *)L"bytes\r\n");

    // カーネルファイルを読み込む
    st->ConOut->OutputString(
        st->ConOut,
        (CHAR16 *)L"Reading kernel file...\r\n");
    st->BootServices->AllocatePool(
        EfiLoaderData,
        kernel_file_size,
        &kernel_file_buf);
    kernel_file->Read(kernel_file, &kernel_file_size, (void *)kernel_file_buf);
    checkResult(result, (CHAR16 *)L"Failed to read kernel file!");

    intToChar16((UINT64)kernel_file_buf, str_int_buf, 16);
    st->ConOut->OutputString(
        st->ConOut,
       (CHAR16 *)L"The kernel was loaded at 0x");
    st->ConOut->OutputString(st->ConOut, str_int_buf);
    st->ConOut->OutputString(st->ConOut, (CHAR16 *)L"\r\n");

    // 実行時のカーネルの最初と最後のアドレスを取得する
    Elf64_Ehdr *kernel_header = (Elf64_Ehdr *)kernel_file_buf;
    UINT64 kernel_start_addr, kernel_last_addr;
    getElfStartLastAddr(kernel_header, &kernel_start_addr, &kernel_last_addr);

    intToChar16(kernel_start_addr, str_int_buf, 16);
    st->ConOut->OutputString(
        st->ConOut,
        (CHAR16 *)L"kernel start address : 0x");
    st->ConOut->OutputString(st->ConOut, str_int_buf);
    st->ConOut->OutputString(st->ConOut, (CHAR16 *)L"\r\n");
    intToChar16(kernel_last_addr, str_int_buf, 16);
    st->ConOut->OutputString(st->ConOut, (CHAR16 *)L"kernel last address : 0x");
    st->ConOut->OutputString(st->ConOut, str_int_buf);
    st->ConOut->OutputString(st->ConOut, (CHAR16 *)L"\r\n");

    // 実行用にカーネルをコピーする
    int requiredPages =
        (kernel_last_addr - kernel_start_addr + 0x3fff) / 0x1000 / 0x4 /*4KB*/;
    st->BootServices->AllocatePages(
        AllocateAddress,
        EfiLoaderData,
        requiredPages,
        &kernel_start_addr);
    memCopyElf(kernel_header, kernel_file_buf);

    // エントリーアドレスを取得・カーネルに渡す引数を準備
    UINT64 entry_addr = kernel_header->e_entry;
    struct PlatformInformation pi;
    pi.fb.base = gop->Mode->FrameBufferBase;
    pi.fb.size = gop->Mode->FrameBufferSize;
    pi.fb.hr = gop->Mode->Info->HorizontalResolution;
    pi.fb.vr = gop->Mode->Info->VerticalResolution;
    switch (gop->Mode->Info->PixelFormat) {
        case PixelRedGreenBlueReserved8BitPerColor:
        case PixelBlueGreenRedReserved8BitPerColor:
            pi.fb.format = gop->Mode->Info->PixelFormat;
            break;
        default:
            st->ConOut->OutputString(
                st->ConOut,
                (CHAR16 *)L"Incompatible pixel format");
            halt();
    }

    intToChar16(entry_addr, str_int_buf, 16);
    st->ConOut->OutputString(st->ConOut, (CHAR16 *)L"entry address : 0x");
    st->ConOut->OutputString(st->ConOut, str_int_buf);
    st->ConOut->OutputString(st->ConOut, (CHAR16 *)L"\r\n");

    // ACPI テーブルを取得する
    pi.rsdp = getAcpiTable();
    if (pi.rsdp == NULL) {
        st->ConOut->OutputString(
            st->ConOut,
            (CHAR16 *)L"Failed to get ACPI Table.");
        halt();
    }

    // ファイル等を閉じる
    st->ConOut->OutputString(st->ConOut, (CHAR16 *)L"Closing files...\r\n");
    kernel_file->Close(kernel_file);
    root->Close(root);

    // メモリマップの取得
    st->ConOut->OutputString(
        st->ConOut,
        (CHAR16 *)L"Getting the memory map...\r\n");
    struct MemoryInfo mem_info = {0, NULL, 0, 0};
    result = getMemoryMap(&mem_info);
    checkResult(result, (CHAR16 *)L"Failed to get the memory map!");

    // BootServicesを抜ける
    st->ConOut->OutputString(
        st->ConOut,
        (CHAR16 *)L"Exiting Boot Services...\r\n");
    result = st->BootServices->ExitBootServices(ImageHandle, mem_info.mapKey);
    if (result == EFI_INVALID_PARAMETER) {
        // 失敗したら再度メモリマップを取得し、ExitBootServicesを再実行
        result = getMemoryMap(&mem_info);
        checkResult(result, (CHAR16 *)L"Failed to get the memory map!");

        result =
            st->BootServices->ExitBootServices(ImageHandle, mem_info.mapKey);
        checkResult(result, (CHAR16 *)L"Could not exit boot service!");
    }

    // カーネルへ移る
	/* 
	   引数の値は次のレジスタへ渡す
	   第1引数 rdi	
	   第2引数 rsi
	   第3引数 rdx
	   第4引数 rcx
	   第5引数 r8
	   第6引数 r9 
	   
	   その後jmpで飛べばOK
	   ここでスタックポインタを設定すべきだが、
	   スタックのアドレスをこのバイナリから判断するのがややこしいため、
	   カーネルのアセンブリ関数で一度設定した後、
	   C言語の関数へ移る
	*/
    EFI_PHYSICAL_ADDRESS pi_addr = (EFI_PHYSICAL_ADDRESS)&pi;
	__asm__ (
		 "mov  %0, %%rdi\n"
		 "jmp  *%1\n"
		 ::
		 "m"(pi_addr),
		 "m"(entry_addr));

    return EFI_SUCCESS;
}
