#include "vector.h"

#include <stdio.h>
#include <math.h>

Vector createVector(const Point *origin, const Point *destination) {
    Vector v = {destination->x - origin->x, destination->y - origin->y, destination->z - origin->z};
    return v;
}

Vector pointToVector(const Point *p) {
    return (Vector){p->x, p->y, p->z};
}

Point vectorToPoint(const Vector *v) {
    return (Point){v->x, v->y, v->z};
}

float dot(const Vector *a, const Vector *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

float dot2D(const Vector *a, const Vector *b) {
    return a->x * b->x + a->y * b->y;
}

float magnitude(const Vector *v) {
    return sqrtf(powf(v->x, 2) + powf(v->y, 2) + powf(v->z, 2));
}

float getAngle(const Vector *a, const Vector *b) {
    return acosf(dot(a, b) / (magnitude(a) * magnitude(b)));
}

Vector subtract(const Vector *a, const Vector *b) {
    Vector result = {a->x - b->x, a->y - b->y, a->z - b->z};
    return result;
}

Vector rejectFrom(const Vector *direction, const Vector *vector) {
    float d = dot(direction, direction);
    if (d == 0) return (Vector){0,0,0};

    Vector proj = scalarMultiply(dot(direction, vector) / d, direction);
    Vector result = subtract(vector, &proj);
    return result;
}

Vector scalarMultiply(float num, const Vector *v) {
    Vector result = {v->x * num, v->y * num, v->z * num };
    return result;
}

Vector crossProduct(const Vector *a, const Vector *b) {
    Vector result;
    result.x = a->y * b->z - a->z * b->y;
    result.y = a->z * b->x - a->x * b->z;
    result.z = a->x * b->y - a->y * b->x;
    return result;
}

Vector normalize(const Vector *v) {
    float length = magnitude(v);

    if (length == 0) {
        printf("Warning: trying to normalize null vector");
        return (Vector) {0, 0, 0};
    }

    Vector result = { v->x / length, v->y / length, v->z / length };
    return result;
}

Vector reflect(const Vector *incident, const Vector *normal) {
    // R = I - 2 * (I · N) * N
    float dot_product = dot(incident, normal);
    Vector scaled_normal = scalarMultiply(2.0f * dot_product, normal);
    Vector reflected = subtract(incident, &scaled_normal);
    return reflected;
}

Vector multiplyMatrixVector(float m[4][4], const Vector *p) {
    Vector result;
    float x = p->x, y = p->y, z = p->z;
    float w = 1;

    result.x = m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3] * w;
    result.y = m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3] * w;
    result.z = m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3] * w;

    return result;
}