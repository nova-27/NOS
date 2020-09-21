//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#pragma once
#include "./efi.h"

// 関数のextern宣言
extern void checkResult(EFI_STATUS, CHAR16 *);
extern void intToChar16(UINT64, CHAR16*, int);
extern void halt();
