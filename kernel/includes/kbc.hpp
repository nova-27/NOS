//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#pragma once

#define ASCII_ESC   0x1b
#define ASCII_BS    0x08
#define ASCII_HT    0x09

extern char getc();
extern void kbcInit(void);
