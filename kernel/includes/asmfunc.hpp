#pragma once

extern "C" {
	void io_out32(int addr, int data);
	int io_in32(int addr);
}
