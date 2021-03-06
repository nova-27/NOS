//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#pragma once
#include "./efi.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// メモリに関する情報
struct MemoryInfo {
    UINTN mapSize;
    EFI_MEMORY_DESCRIPTOR *map;
    UINTN descriptorSize;
    UINTN mapKey;
};

// フレームバッファに関する情報
struct FrameBuffer {
    EFI_PHYSICAL_ADDRESS base;
    UINTN size;
    UINT32 hr;
    UINT32 vr;
    EFI_GRAPHICS_PIXEL_FORMAT format;
};

// プラットフォーム情報
struct PlatformInformation {
    struct FrameBuffer fb;
    void* rsdp;
};
