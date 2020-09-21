//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#pragma once

extern "C" {
    void ioOut32(int addr, int data);
    int ioIn32(int addr);
    void defaultHandler();
    void timerHandler();
}
