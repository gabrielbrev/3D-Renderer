#ifndef SURFACE_H
#define SURFACE_H

#include "vector.h"
#include "color.h"

typedef struct {
    int size;
    Point **control_points;
    float **weights;
    ColorHSV color;
} Surface;

Surface createSurface3x3(Point control_points[3][3], float weights[3][3], ColorHSV color);
Point evaluateSurface(const Surface *surf, float s, float t);

#endif
