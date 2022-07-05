#include <cmath>
#include "GamepadScreen.h"

int GamepadScreen::screenIndex(int spriteIndex, int spriteWidth, int xOff, int yOff) const {

    return screenWidth * (yOff + floor(spriteIndex / spriteWidth)) + (spriteIndex % spriteWidth) + xOff;
}

int GamepadScreen::screenIndex(int x, int y) const {
    return 4 * (screenWidth * y + x);
}

void GamepadScreen::setPixel(int screenIndex, const drc::byte *colorBytes) {
    for (int i = 0; i < 4; i++) {
        pixels->at(screenIndex + i) = colorBytes[i];
    }
}

GamepadScreen::GamepadScreen(int width, int height) {
    screenWidth = width;
    screenHeight = height;
    pixels = new std::vector<drc::byte>(width * height * 4, 1);
}

void GamepadScreen::horLine(int x1, int x2, int y, const drc::byte *colorBytes) {
    while (x1 <= x2) {
        setPixel(screenIndex(x1++, y), colorBytes);
    }
}

void GamepadScreen::verLine(int x, int y1, int y2, const drc::byte *colorBytes) {
    while (y1 <= y2) {
        setPixel(screenIndex(x, y1++), colorBytes);
    }
}

void GamepadScreen::circle2(int xc, int yc, int thickness, int radius, const drc::byte *colorBytes) {
    int inner = radius - thickness + 1;
    int xo = radius;
    int xi = inner;
    int y = 0;
    int erro = 1 - xo;
    int erri = 1 - xi;

    while (xo >= y) {
        horLine(xc + xi, xc + xo, yc + y, colorBytes);
        verLine(xc + y, yc + xi, yc + xo, colorBytes);
        horLine(xc - xo, xc - xi, yc + y, colorBytes);
        verLine(xc - y, yc + xi, yc + xo, colorBytes);
        horLine(xc - xo, xc - xi, yc - y, colorBytes);
        verLine(xc - y, yc - xo, yc - xi, colorBytes);
        horLine(xc + xi, xc + xo, yc - y, colorBytes);
        verLine(xc + y, yc - xo, yc - xi, colorBytes);

        y++;

        if (erro < 0) {
            erro += 2 * y + 1;
        } else {
            xo--;
            erro += 2 * (y - xo + 1);
        }

        if (y > inner) {
            xi = y;
        } else {
            if (erri < 0) {
                erri += 2 * y + 1;
            } else {
                xi--;
                erri += 2 * (y - xi + 1);
            }
        }
    }
}

/*
 * based on http://members.chello.at/~easyfilter/bresenham.html
 */
void GamepadScreen::drawLine(int x0, int y0, int x1, int y1, float width, uint32_t color) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx - dy, e2, x2, y2;                          /* error value e_xy */
    float ed = dx + dy == 0 ? 1 : sqrt((float) dx * dx + (float) dy * dy);

    drc::byte colorBytes[4];
    for (unsigned char &colorByte: colorBytes) {
        colorByte = color & 0xFF;
        color = color >> 8;
    }


    for (width = (width + 1) / 2;;) {
        setPixel(screenIndex(x0, y0), colorBytes);
        e2 = err;
        x2 = x0;
        if (2 * e2 >= -dx) {
            for (e2 += dy, y2 = y0; e2 < ed * width && (y1 != y2 || dx > dy); e2 += dx) {
                setPixel(screenIndex(x0, y2 += sy), colorBytes);
            }
            if (x0 == x1) {
                break;
            }
            e2 = err;
            err -= dy;
            x0 += sx;
        }
        if (2 * e2 <= dy) {
            for (e2 = dx - e2; e2 < ed * width && (x1 != x2 || dx < dy); e2 += dy) {
                setPixel(screenIndex(x2 += sx, y0), colorBytes);
            }
            if (y0 == y1) {
                break;
            }
            err += dx;
            y0 += sy;
        }
    }
}

std::vector<drc::byte> *GamepadScreen::getPixels() {
    return this->pixels;
}