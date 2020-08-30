#include "segmentation.hpp"

namespace segmentation {
    //GDTRレジスタに入れる変数
    struct {
        unsigned short     size;
        unsigned long long *base;
    } __attribute__((packed)) gdtr;

    //Global Descriptor Table
    unsigned long long gdt[] = {
        0b00000000'0'0'0'0'0000'0'00'0'0000'00000000'0000000000000000'0000000000000000, /* NULL descriptor */
        0b00000000'1'0'1'0'1111'1'00'1'1010'00000000'0000000000000000'1111111111111111, /* Base=0 Limit=4GB KernelLevel Code(Read Execute) */
        0b00000000'1'0'1'0'1111'1'00'1'0010'00000000'0000000000000000'1111111111111111, /* Base=0 Limit=4GB KernelLevel Data(Read Write) */
    };

    //セグメンテーションの設定をする
    void init() {
        //GDTRレジスタを設定
        gdtr.size = 3 * 64;
        gdtr.base = gdt;

        __asm__ volatile(
            "lgdt %0"
            :: "m"(gdtr)
        );

        //データセグメント・スタックセグメントの設定
        __asm__ volatile(
            "mov %0, %%ds;"
            "mov %0, %%ss;"
            "mov %0, %%es;"
            "mov %0, %%fs;"
            "mov %0, %%gs;"
            :: "r"(2 << 3)
        );

        //コードセグメントの設定
        unsigned long long code_segment = 1 << 3;
        __asm__ volatile(
            "leaq .next, %%rax;"
            "pushq %0;"
            "pushq %%rax;"
            "lretq;"
            ".next:"
            :: "m"(code_segment)
        );
    }
}