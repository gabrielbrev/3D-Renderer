#include "point.h"
#include "vector.h"

#include <stdio.h>
#include <math.h>

Vector createVector(Point origin, Point destination) {
    Vector v = {destination.x - origin.x, destination.y - origin.y, destination.z - origin.z};
    return v;
}

float dot(Vector a, Vector b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector subtract(Vector a, Vector b) {
    Vector result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return result;
}

Vector rejectFrom(Vector direction, Vector vector) {
    Vector result = subtract(vector, scalarMultiply(dot(direction, vector) / dot(direction, direction), direction));
    return result;
}

Vector scalarMultiply(float num, Vector v) {
    Vector result = {v.x * num, v.y * num, v.z * num };
    return result;
}

Vector crossProduct(Vector a, Vector b) {
    Vector result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

Vector normalize(Vector v) {
    float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

    if (length == 0) {
        printf("Warning: trying to normalize null vector");
        return (Vector) {0, 0, 0};
    }

    Vector result = { v.x / length, v.y / length, v.z / length };
    return result;
}

Vector multiplyMatrixVector(float m[4][4], Vector p) {
    Vector result;
    float x = p.x, y = p.y, z = p.z;
    float w = 1;

    result.x = m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3] * w;
    result.y = m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3] * w;
    result.z = m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3] * w;
    w = m[3][0] * x + m[3][1] * y + m[3][2] * z + m[3][3] * w;

    return result;
}