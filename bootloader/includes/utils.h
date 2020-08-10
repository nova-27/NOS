#pragma once
#include "efi.h"

//関数のプロトタイプ宣言
extern void checkResult(EFI_STATUS, CHAR16 *);
extern void intToChar16(UINT64, CHAR16*, int);
extern void halt(void);