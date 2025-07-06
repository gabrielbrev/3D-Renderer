#ifndef VECTOR_H
#define VECTOR_H

typedef union {
    struct { float x, y, z; };
    float data[3];
} Vector;

typedef Vector Point;

Vector createVector(const Point *origin, const Point *destination);
float dot(const Vector *a, const Vector *b);
float dot2D(const Vector *a, const Vector *b);
float magnitude(const Vector *v);
float getAngle(const Vector *a, const Vector *b);
Vector subtract(const Vector *a, const Vector *b);
Vector scalarMultiply(float num, const Vector *v);
Vector rejectFrom(const Vector *direction, const Vector *vector);
Vector crossProduct(const Vector *a, const Vector *b);
Vector normalize(const Vector *v);
Vector reflect(const Vector *incident, const Vector *normal);
Vector multiplyMatrixVector(float m[4][4], const Vector *p);

#endif