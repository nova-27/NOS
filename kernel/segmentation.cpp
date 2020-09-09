/* Copyright (C) 2020 nova27. All rights reserved. */

#include <stdlib.h>

namespace segmentation {
    // GDTRレジスタに入れる変数
    struct {
        u_int16_t  size;
        u_int64_t  *base;
    } __attribute__((packed)) gdtr;

    // Global Descriptor Table
    u_int64_t gdt[] = {
        // NULL descriptor
        0x00'0'0'0'0'00'0000'0000,
        // Base=0 Limit=4GB KernelLevel Code(Read Execute)
        0x00'A'F'9'A'00'0000'FFFF,
        // Base=0 Limit=4GB KernelLevel Data(Read Write)
        0x00'A'F'9'2'00'0000'FFFF,
    };

    // セグメンテーションの設定をする
    void init() {
        // GDTRレジスタを設定
        gdtr.size = 3 * 64;
        gdtr.base = gdt;

        __asm__ volatile(
            "lgdt %0"
            :: "m"(gdtr)
        );

        // データセグメント・スタックセグメントの設定
        __asm__ volatile(
            "mov %0, %%ds;"
            "mov %0, %%ss;"
            "mov %0, %%es;"
            "mov %0, %%fs;"
            "mov %0, %%gs;"
            :: "r"(2 << 3)
        );

        // コードセグメントの設定
        u_int64_t code_segment = 1 << 3;
        __asm__ volatile(
            "leaq .next, %%rax;"
            "pushq %0;"
            "pushq %%rax;"
            "lretq;"
            ".next:"
            :: "m"(code_segment)
        );
    }
}  // namespace segmentation
