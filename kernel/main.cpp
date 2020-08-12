#include "../bootloader/includes/common.h"
#include "graphics.hpp"

//スタック用変数
alignas(16) unsigned char kernel_stack[1024 * 1024];

/* エントリポイント */
extern "C" void KernelMain(struct fb *fb) {
	graphics screen(fb);
	struct color color;
	
	color.red = 0x41;
	color.green = 0x96;
	color.blue = 0xE1;
	screen.fill(1, 1, fb->hr, fb->vr, color);

	color.red = 0x00;
	color.green = 0xFF;
	color.blue = 0x00;
	screen.fill(250, 250, 450, 450, color);

	color.red = 0x00;
	color.green = 0x00;
	color.blue = 0xFF;
	screen.fill(400, 400, 600, 600, color);

	return;
}