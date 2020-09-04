#pragma once

extern "C" {
	extern void io_out32(int addr, int data);
	extern int io_in32(int addr);
	extern void default_handler();
}
