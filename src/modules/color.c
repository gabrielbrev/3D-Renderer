#include "color.h"

#include <math.h>

ColorRGB HSVtoRGB(const ColorHSV *hsv, float intensity) {
    ColorRGB rgb;
    float h = hsv->h;
    float s = hsv->s;
    float v = hsv->v;

    if (s == 0.0f) {
        rgb.r = rgb.g = rgb.b = (int)(v * 255 * intensity);
        return rgb;
    }

    h = fmodf(h, 360.0f); 
    float c = v * s;
    float x = c * (1 - fabsf(fmodf(h / 60.0f, 2) - 1));
    float m = v - c;

    float r, g, b;

    if (h < 60) {
        r = c; g = x; b = 0;
    } else if (h < 120) {
        r = x; g = c; b = 0;
    } else if (h < 180) {
        r = 0; g = c; b = x;
    } else if (h < 240) {
        r = 0; g = x; b = c;
    } else if (h < 300) {
        r = x; g = 0; b = c;
    } else {
        r = c; g = 0; b = x;
    }

    rgb.r = (int)((r + m) * 255 * intensity);
    rgb.g = (int)((g + m) * 255 * intensity);
    rgb.b = (int)((b + m) * 255 * intensity);

    return rgb;
}

Uint32 HSVtoUint32(const ColorHSV *hsv, float intensity) {
    ColorRGB rgb = HSVtoRGB(hsv, intensity);
    Uint32 color = (0xFF << 24) | (rgb.r << 16) | (rgb.g << 8) | rgb.b;
    return color;
}  