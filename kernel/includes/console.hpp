//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#pragma once

#include "../../bootloader/includes/common.h"
#include "graphics.hpp"

#define FONT_WIDTH 8
#define FONT_HEIGHT 10
#define FONT_DISTANCE 1

extern char font_bitmap[0x7f][FONT_HEIGHT][FONT_WIDTH + 1];

// (baseX, baseY)から文字を出力するコンソールを作成します
// 使用例 :
//    Console console(&graphics, 10, 10);
//    console.setColor(color);
//    console.putChar('A');
//    console.putString("ABCDEFG");
//
// nowX, nowY は次に書き始める文字の位置を表します
// 文字の間隔は、横がFONT_DISTANCEピクセル分、縦がFONTDISTANCE*2ピクセル分です
// 画面の横からはみ出しそうになると、自動的に改行します
// 画面の縦からはみ出しそうになった時の動作は未定義です（意図しない位置にデータを書き込む可能性あり）
class Console {
 private:
    Graphics *screen_;

    const int base_x_;
    const int base_y_;

    int now_x_;
    int now_y_;

    Color color_;

 public:
    // コンストラクタ
    // Graphicsクラスのオブジェクトはデストラクタが呼ばれても保持し続けます
    Console(Graphics*, int, int);
    // 文字色を設定します
    void setColor(Color);
    // 一文字を出力します
    bool putChar(char);
    // 文字列を出力します
    void putString(const char*);
};
