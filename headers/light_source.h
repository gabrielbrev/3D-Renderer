#ifndef LIGHT_H
#define LIGHT_H

#include "vector.h"
#include "mesh.h"

typedef struct {
    Point pos;

    float diffuse_intensity;
    float ambient_intensity;

    float attenuationQuadratic;  // a
    float attenuationLinear;     // b
    float attenuationConstant;   // c
} LightSource;

float getFaceLightIntensity(const LightSource *light, const Point *a, const Point *b, const Point *c, Material *material);

#endif