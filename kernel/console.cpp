//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#include "console.hpp"

// コンストラクタ
Console::Console(
    Graphics* screen,
    int baseX,
    int baseY
) : screen_(screen), baseX_(baseX), baseY_(baseY) {
    nowX_ = baseX;
    nowY_ = baseY;
    color color;
    color.red = 0xff;
    color.green = 0xff;
    color.blue = 0xff;
    color_ = color;
}

// 文字色を設定する
void Console::setColor(color color) {
    color_ = color;
}

// 文字を出力する
bool Console::putchar(char character) {
    if (nowX_ + FONT_WIDTH > screen_->getHr()) {
        // 横がはみ出すなら
        nowX_ = baseX_;
        nowY_ += FONT_HEIGHT + FONT_DISTANCE * 2;
    }

    // font_bitmap_の*になっている部分は描画、それ以外(空白)は描画しない
    for (int x = 0; x < FONT_WIDTH; x++) {
        for (int y = 0; y < FONT_HEIGHT; y++) {
            if (font_bitmap_[character][y][x] == '*')
                screen_->drawPixel(x + nowX_, y + nowY_, color_);
        }
    }

    // 次描く位置を右へずらす
    nowX_ += FONT_WIDTH + FONT_DISTANCE;

    return true;
}

// 文字列を出力する
void Console::putString(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        putchar(str[i]);
    }
}
