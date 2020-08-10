#include "utils.h"

/* 戻り値をチェックする */
void checkResult(EFI_SYSTEM_TABLE *ST, EFI_STATUS result, CHAR16 *String)  {
	if(result != EFI_SUCCESS) {
		ST->ConOut->OutputString(ST->ConOut, String);
		halt();
	}
}

/* 整数を文字列にする */
void intToChar16(UINT64 num, CHAR16 *result, int redix)
{
	// 桁数を求める
	int digit = 1;
	for (UINT64 i = num; i >= redix; i /= redix)
	{
		digit++;
	}

	// 最後はnull文字
	result[digit] = '\0';

	// 一の位から求めていく
	UINT64 tmp = num;
	for (int i = digit; i > 0; i--)
	{
		int numOfPos = tmp % redix;
		if (numOfPos <= 9)
		{
			// 0から9の範囲
			result[i - 1] = numOfPos + L'0';
		}
		else
		{
			//A(0xA)からの範囲
			result[i - 1] = numOfPos - 10 + L'A';
		}
		tmp /= redix;
	}
}

/* 処理を停止 */
void halt() {
	while(1) __asm__("hlt");
}