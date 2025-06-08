#include "triangle.h"
#include "utils.h"

SDL_Rect getBoundingRect(const Point *a, const Point *b, const Point *c) {
    int x = minInt(minInt(a->x, b->x), c->x);
    int y = minInt(minInt(a->y, b->y), c->y);

    int w = maxInt(maxInt(a->x, b->x), c->x) - x;
    int h = maxInt(maxInt(a->y, b->y), c->y) - y;

    return (SDL_Rect) {x, y, w, h};
}