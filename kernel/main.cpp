//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#include <string>
#include "../bootloader/includes/common.h"
#include "graphics.hpp"
#include "console.hpp"
#include "serial_port.hpp"
#include "segmentation.hpp"
#include "interrupt.hpp"
#include "kbc.hpp"

// スタック用変数
alignas(16) unsigned char kernel_stack[1024 * 1024];

void intToChar(uint64_t num, char *result, int redix) {
    // 桁数を求める
    int digit = 1;
    for (int i = num; i >= redix; i /= redix) {
        digit++;
    }

    // 最後はnull文字
    result[digit] = '\0';

    // 一の位から求めていく
    uint64_t tmp = num;
    for (int i = digit; i > 0; i--) {
        int num_of_pos = tmp % redix;
        if (num_of_pos <= 9) {
            // 0から9の範囲
            result[i - 1] = num_of_pos + '0';
        } else {
            // A(0xA)からの範囲
            result[i - 1] = num_of_pos - 10 + 'A';
        }
        tmp /= redix;
    }
}

// エントリポイント
extern "C" void kernelMain(struct PlatformInformation *pi) {
    // セグメントを初期化
    segmentation::init();
    // 割り込みを有効化
    interrupt::idtrInit();
    interrupt::picInit();

	kbc::init();

    Graphics graphics(&pi->fb);
    Console console(&graphics, 10, 10);

    SerialPort serial_port(COM1);

    console.putString("ABCD");

    // acpi::init(pi->rsdp);
    // acpi_timer::Init();
    // apic_timer::Init();

    __asm__("sti");

    // char buf[10];
    // unsigned short vendor;
    // unsigned short deviceID;

    // for(int bus = 0; bus <= 255; bus++) {
        // for(int slot = 0; slot <= 31; slot++) {
            // vendor = pci_manager.PCIConfigReadWord(bus, slot, 0, 0);
            // if(vendor != 0xFFFF) {
                // intToChar(vendor, buf, 16);
                // console.putString(buf);
                // console.putString(":");

                // deviceID = pci_manager.PCIConfigReadWord(bus, slot, 0, 2);
                // intToChar(deviceID, buf, 16);
                // console.putString(buf);
                // console.putString(" ");
            // }
        // }
    // }
//
    return;
}