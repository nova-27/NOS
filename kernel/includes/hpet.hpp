//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#pragma once
#include "acpi.hpp"

namespace hpet {
    extern void init();
    extern void sleep(u_int64_t);

    extern u_int64_t reg_base;
}  // namespace hpet
