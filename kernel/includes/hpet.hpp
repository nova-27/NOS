#pragma once
#include "acpi.hpp"

namespace hpet{
    extern void init();
    extern void sleep(unsigned long long);

    extern unsigned long long reg_base;
}