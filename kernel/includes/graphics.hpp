//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#pragma once
#include "../../bootloader/includes/common.h"

// 色の情報を入れる構造体
typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} Color;

// 1ピクセルの情報
struct Pixel {
    char color1;
    char color2;
    char color3;
    char reserve;
};

// 画面に直接描画します
// 使用例 :
//    Graphics screen(&fb);
//    screen.drawPixel(1, 1, red);
//    screen.fill(1, 1, 10, 10, blue);
//    int width = screen.getHr();
//
// 範囲外に書き込まないようにif文でチェックしている（はず）
class Graphics {
 private:
    const FrameBuffer *fb_;
 public:
    // コンストラクタ
    explicit Graphics(FrameBuffer*);
    // 一ピクセル分を描画する
    bool drawPixel(int, int, Color);
    // 四角形を描画する
    bool fill(int, int, int, int, Color);
    // 横幅を取得する
    int getHr() const;
};
