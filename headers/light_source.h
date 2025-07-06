#ifndef LIGHT_H
#define LIGHT_H

#include "vector.h"
#include "mesh.h"
#include "color.h"

typedef struct {
    Point pos;

    float diffuse_intensity;
    float ambient_intensity;
    float specular_intensity;

    ColorRGB color;

    float attenuationQuadratic;  // a
    float attenuationLinear;     // b
    float attenuationConstant;   // c
} LightSource;

float getFaceLightIntensity(const LightSource *light, const Point *a, const Point *b, const Point *c, Material *material);

float getPhongLightIntensity(const LightSource *light, const Point *a, const Point *b, const Point *c, const Point *camera_pos, Material *material);

ColorRGB getPhongColor(const LightSource *light, const Point *a, const Point *b, const Point *c, const Point *camera_pos, Material *material, const ColorHSV *surface_color);

#endif