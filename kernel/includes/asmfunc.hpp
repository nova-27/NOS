//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#pragma once

extern "C" {
    void io_out32(int addr, int data);
    int io_in32(int addr);
    void default_handler();
}
