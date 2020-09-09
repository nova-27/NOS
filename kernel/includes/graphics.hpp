/* Copyright (C) 2020 nova27. All rights reserved. */

#pragma once
#include "../../bootloader/includes/common.h"

// 色の情報を入れる構造体
typedef struct {
    char red;
    char green;
    char blue;
} color;

// 1ピクセルの情報
struct pixel {
    char color1;
    char color2;
    char color3;
    char reserve;
};

// クラス
class graphics {
 private:
    const fb *m_fb;
 public:
    explicit graphics(fb*);
    bool drawPixel(int, int, color);
    bool fill(int, int, int, int, color);
    int getHr() const;
};
