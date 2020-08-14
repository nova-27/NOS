#include "../bootloader/includes/common.h"
#include "graphics.hpp"
#include "console.hpp"

//スタック用変数
alignas(16) unsigned char kernel_stack[1024 * 1024];

/* エントリポイント */
extern "C" void KernelMain(struct fb *fb) {
	//graphics *screen = new graphics(fb);
	//console *mainConsole = new console(screen, 10, 10);

	graphics graphics(fb);
	console console(&graphics, 10, 10);

	//mainConsole->putchar('A');
	//mainConsole->putchar('B');

	color color;
	color.red = 0xFF;
	color.green = 0xFF;
	color.blue = 0x00;
	//(&graphics)->fill(10, 10, 100, 100, color);

	console.putString("!\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMN");


	return;
}