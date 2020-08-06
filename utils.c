#include "utils.h"

/* 整数を文字列にする */
void intToChar16(int num, CHAR16 *result, int redix)
{
	// 桁数を求める
	int digit = 1;
	for (int i = num; i >= redix; i /= redix)
	{
		digit++;
	}

	// 最後はnull文字
	result[digit] = '\0';

	// 一の位から求めていく
	int tmp = num;
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