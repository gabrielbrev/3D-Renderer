#ifndef VECTOR_H
#define VECTOR_H

#include "point.h"

typedef union {
    struct { float x, y, z; };
    float data[3];
} Vector;

Vector createVector(Point origin, Point destination);
float dot(Vector a, Vector b);
Vector subtract(Vector a, Vector b);
Vector scalarMultiply(float num, Vector v);
Vector rejectFrom(Vector direction, Vector vector);
Vector crossProduct(Vector a, Vector b);
Vector normalize(Vector v);
Vector multiplyMatrixVector(float m[4][4], Vector p);

#endif