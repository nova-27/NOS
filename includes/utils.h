#pragma once
#include "efi.h"

//関数のプロトタイプ宣言
extern void checkResult(EFI_SYSTEM_TABLE *, EFI_STATUS, CHAR16 *);
extern void intToChar16(int, CHAR16*, int);
extern void halt(void);