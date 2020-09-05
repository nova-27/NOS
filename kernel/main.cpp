#include "../bootloader/includes/common.h"
#include "graphics.hpp"
#include "console.hpp"
#include "asmfunc.hpp"
#include "segmentation.hpp"
#include "acpi.hpp"
#include "interrupt.hpp"
#include "hpet.hpp"
#include <string>

//スタック用変数
alignas(16) unsigned char kernel_stack[1024 * 1024];

void intToChar(unsigned long long num, char *result, int redix)
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
	unsigned long long tmp = num;
	for (int i = digit; i > 0; i--)
	{
		int numOfPos = tmp % redix;
		if (numOfPos <= 9)
		{
			// 0から9の範囲
			result[i - 1] = numOfPos + '0';
		}
		else
		{
			//A(0xA)からの範囲
			result[i - 1] = numOfPos - 10 + 'A';
		}
		tmp /= redix;
	}
}

/* エントリポイント */
extern "C" void KernelMain(struct platform_information *pi) {
	//セグメントを初期化
	segmentation::init();
	//割り込みを有効化
	interrupt::idtr_init();
	interrupt::pic_init();
	__asm__ ("sti");

	graphics graphics(&pi->fb);
	console console(&graphics, 10, 10);
	
	
	acpi::init(pi->rsdp);
	hpet::init();

	console.putString("AAA");
	hpet::sleep(5000000);
	console.putString("BBB");
	hpet::sleep(5000000);
	console.putString("CCC");
	hpet::sleep(5000000);
	console.putString("DDD");
	hpet::sleep(5000000);
	console.putString("EEE");
	hpet::sleep(5000000);

	//pciManager pci_manager;

	//mainConsole->putchar('A');
	//mainConsole->putchar('B');

	/*char buf[10];
	unsigned short vendor;
  	unsigned short deviceID;

  	for(int bus = 0; bus <= 255; bus++) {
    	for(int slot = 0; slot <= 31; slot++) {
      		if((vendor = pci_manager.PCIConfigReadWord(bus, slot, 0, 0)) != 0xFFFF) {
				intToChar(vendor, buf, 16);
				console.putString(buf);
				console.putString(":");

				deviceID = pci_manager.PCIConfigReadWord(bus, slot, 0, 2);
				intToChar(deviceID, buf, 16);
				console.putString(buf);
				console.putString(" ");
			}
    	}
  	}

	std::string a = "";*/

	return;
}