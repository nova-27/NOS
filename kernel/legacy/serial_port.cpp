//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#include "serial_port.hpp"
#include "asmfunc.hpp"

// コンストラクタ
SerialPort::SerialPort(uint16_t port) : port_(port) {
    ioOut8(port_ + 1, 0x00);  // Disable all interrupts
    ioOut8(port_ + 3, 0x80);  // Enable DLAB (set baud rate divisor)
    ioOut8(port_ + 0, 0x03);  // Set divisor to 3 (lo byte) 38400 baud
    ioOut8(port_ + 1, 0x00);  //                  (hi byte)
    ioOut8(port_ + 3, 0x03);  // 8 bits, no parity, one stop bit
    ioOut8(port_ + 2, 0xC7);  // Enable FIFO, clear them, with 14-byte threshold
    ioOut8(port_ + 4, 0x0B);  // IRQs enabled, RTS/DSR set
}

// バッファが空か確認する
int SerialPort::isTransmitEmpty() {
    return ioIn8(port_ + 5) & 0x20;
}

// 文字を送信する
void SerialPort::writeSerial(char character) {
    while (isTransmitEmpty() == 0) {}
    ioOut8(port_, character);
}

// 文字列を送信する
void SerialPort::writeStringSerial(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        writeSerial(str[i]);
    }
}
