#include "color.h"

#include <math.h>

ColorRGB HSVtoRGBFloat(const ColorHSV *hsv, float intensity) {
    ColorRGB rgb;
    float h = hsv->h;
    float s = hsv->s;
    float v = hsv->v;

    if (s == 0.0f) {
        rgb.r = rgb.g = rgb.b = v * intensity;
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

    rgb.r = (r + m) * intensity;
    rgb.g = (g + m) * intensity;
    rgb.b = (b + m) * intensity;

    return rgb;
}

Uint32 HSVtoUint32(const ColorHSV *hsv, float intensity) {
    ColorRGB rgb = HSVtoRGBFloat(hsv, intensity);
    Uint8 r = (Uint8)(rgb.r * 255.0f);
    Uint8 g = (Uint8)(rgb.g * 255.0f);
    Uint8 b = (Uint8)(rgb.b * 255.0f);
    return (0xFF << 24) | (r << 16) | (g << 8) | b;
}  

Uint32 ColorRGBToUint32(const ColorRGB *color) {
    Uint8 r = (Uint8)(color->r * 255.0f);
    Uint8 g = (Uint8)(color->g * 255.0f);
    Uint8 b = (Uint8)(color->b * 255.0f);
    
    return (0xFF << 24) | (r << 16) | (g << 8) | b;
}