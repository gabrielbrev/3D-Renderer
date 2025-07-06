#ifndef COLOR_H
#define COLOR_H

#include <SDL3/SDL.h>

typedef struct {
    float h;
    float s;
    float v;
} ColorHSV;

typedef struct {
    float r;
    float g;
    float b;
} ColorRGB;

ColorRGB HSVtoRGBFloat(const ColorHSV *hsv, float intensity);
Uint32 HSVtoUint32(const ColorHSV *hsv, float intensity);
Uint32 ColorRGBToUint32(const ColorRGB *color);

#endif