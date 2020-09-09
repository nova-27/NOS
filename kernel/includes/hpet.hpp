/* Copyright (C) 2020 nova27. All rights reserved. */

#pragma once
#include "acpi.hpp"

namespace hpet {
    extern void init();
    extern void sleep(u_int64_t);

    extern u_int64_t reg_base;
}  // namespace hpet
