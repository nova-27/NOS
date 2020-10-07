//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#pragma once

#include "acpi.hpp"

// ACPI PM Timer
namespace acpi_timer {
    // 初期化
    extern void init();
    // タイムの取得
    extern uint32_t getTime();
}  // namespace acpi_timer
