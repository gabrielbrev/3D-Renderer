#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"
#include "color.h"

#include <SDL3/SDL.h>

typedef struct {
   int vertices[3];
   ColorHSV color;
} Triangle;

SDL_Rect getBoundingRect(const Point *a, const Point *b, const Point *c);
Vector getNormal(const Point *a, const Point *b, const Point *c);
Point getCentroid(const Point *a, const Point *b, const Point *c);

#endif