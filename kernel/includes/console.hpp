#pragma once
#include "../../bootloader/includes/common.h"
#include "graphics.hpp"

#define FONT_WIDTH 8
#define FONT_HEIGHT 10
#define FONT_DISTANCE 1

extern char FONT_BITMAP[0x7f][FONT_HEIGHT][FONT_WIDTH + 1]; 

class console {
private:
    graphics *m_screen;

    const int m_baseX;
    const int m_baseY;

    int m_nowX;
    int m_nowY;

    color m_color;
public:
    console(graphics*, int, int);
    void setColor(color);
    bool putchar(char);
    void putString(const char*);
};