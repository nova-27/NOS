#include "graphics.hpp"

//コンストラクタ
graphics::graphics(fb *fb) : m_fb(fb) {}

//1ピクセル描画する
bool graphics::drawPixel(int x, int y, color color) {
    //(1, 1)ピクセルがm_fb->baseになるから-1する必要がある
    x--;
    y--;

    pixel *base = (pixel *)m_fb->base;
    pixel *pixel = base + (m_fb->hr * y) + x;

    //範囲外だったらfalse
    if((unsigned long long)pixel >= m_fb->base + m_fb->size) return false; 

    //描画する
    switch(m_fb->format) {
        //RGBの順なら
        case PixelRedGreenBlueReserved8BitPerColor:
            pixel->color1 = color.red;
            pixel->color2 = color.green;
            pixel->color3 = color.blue;
            break;
        //BGRの順なら
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

//(fromX, fromY)から(toX, toY)まで単色で埋める
bool graphics::fill(int fromX, int fromY, int toX, int toY, color color) {
    int minX = MIN(fromX, toX);
    int maxX = MAX(fromX, toX);
    int minY = MIN(fromY, toY);
    int maxY = MAX(fromY, toY);

    for(int x = minX; x <= maxX; x++) {
        for(int y = minY; y <= maxY; y++) {
            drawPixel(x, y, color);
        }
    }
}