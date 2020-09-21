//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#pragma once

#define COM1 0x3f8
#include <cstdint>
#include <string>

// シリアル通信をします
// 使用例 :
//    SerialPort serial_port(COM1);
//    serial_port.writeSerial('A');
//
// レガシーデバイス対応
// デバッグにしか使わない予定
class SerialPort {
 private:
    uint16_t port_;
 public:
    // コンストラクタ
    // 第一引数 ioポートアドレスを渡す
    explicit SerialPort(uint16_t);
    // バッファが空か確認する
    int isTransmitEmpty();
    // 文字を送信する
    // 第一引数 送信する文字
    void writeSerial(char);
    // 文字列を送信する
    // 第一引数 送信する文字列
    void writeStringSerial(char*);
};
