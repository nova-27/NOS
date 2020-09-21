//
// Nova27's Operating System
//
// Copyright (c) 2020 nova27
//
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#include "graphics.hpp"
#include <cstdint>

// コンストラクタ
Graphics::Graphics(FrameBuffer *fb) : fb_(fb) {}

// 1ピクセル描画する
bool Graphics::drawPixel(int x, int y, color color) {
    // (1, 1)ピクセルがm_fb->baseになるから-1する必要がある
    x--;
    y--;

    Pixel *base = reinterpret_cast<Pixel*>(fb_->base);
    Pixel *pixel = base + (fb_->hr * y) + x;

    // 範囲外だったらfalse
    if ((uint64_t)pixel < fb_->base) return false;
    if ((uint64_t)pixel >= fb_->base + fb_->size) return false;

    // 描画する
    switch (fb_->format) {
        // RGBの順なら
        case PixelRedGreenBlueReserved8BitPerColor:
            pixel->color1 = color.red;
            pixel->color2 = color.green;
            pixel->color3 = color.blue;
            break;
        // BGRの順なら
        case PixelBlueGreenRedReserved8BitPerColor:
            pixel->color1 = color.blue;
            pixel->color2 = color.green;
            pixel->color3 = color.red;
            break;
        default:
            return false;
    }

    return true;
}

// (fromX, fromY)から(toX, toY)まで単色で埋める
bool Graphics::fill(int fromX, int fromY, int toX, int toY, color color) {
    int minX = MIN(fromX, toX);
    int maxX = MAX(fromX, toX);
    int minY = MIN(fromY, toY);
    int maxY = MAX(fromY, toY);

    for (int x = minX; x <= maxX; x++) {
        for (int y = minY; y <= maxY; y++) {
            bool result = drawPixel(x, y, color);
            if (!result) return false;
        }
    }

    return true;
}

// 画面の横幅を取得する
int Graphics::getHr() const {
    return fb_->hr;
}
