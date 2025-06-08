#ifndef COLOR_H
#define COLOR_H

#include <SDL3/SDL.h>

typedef struct {
    float h;
    float s;
    float v;
} ColorHSV;

typedef struct {
    Uint8 r;
    Uint8 b;
    Uint8 g;
} ColorRGB;

ColorRGB HSVtoRGB(const ColorHSV *hsv);
Uint32 HSVtoUint32(const ColorHSV *hsv);

#endif