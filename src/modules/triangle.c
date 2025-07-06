#include "triangle.h"
#include "utils.h"

SDL_Rect getBoundingRect(const Point *a, const Point *b, const Point *c) {
    int x = minInt(minInt(a->x, b->x), c->x);
    int y = minInt(minInt(a->y, b->y), c->y);

    int w = maxInt(maxInt(a->x, b->x), c->x) - x;
    int h = maxInt(maxInt(a->y, b->y), c->y) - y;

    return (SDL_Rect) {x, y, w, h};
}

Vector getNormal(const Point *a, const Point *b, const Point *c) {
    Vector u = createVector(a, b);
    Vector v = createVector(a, c);

    Vector normal = crossProduct(&u, &v);
    return normalize(&normal);
}

Point getCentroid(const Point *a, const Point *b, const Point *c) {
    Point center;
    center.x = (a->x + b->x + c->x) / 3.0f;
    center.y = (a->y + b->y + c->y) / 3.0f;
    center.z = (a->z + b->z + c->z) / 3.0f;
    return center;
}
