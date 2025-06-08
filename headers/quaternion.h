#ifndef QUATERNION_H
#define QUATERNION_H

#include "vector.h"
#include "utils.h"

typedef union {
    struct { float w, x, y, z; };
    float data[4];
} Quaternion;

Quaternion vecToQuaternion(const Vector *vector);
Quaternion pointToQuaternion(const Point *point);
Quaternion createRotationQuaternion(const Vector *vector, float radians);
Quaternion multiply(const Quaternion *a, const Quaternion *b);
Quaternion getConjugate(const Quaternion *quaternion);
Vector rotateVector(const Vector *target, const Vector *axis, float degrees);
Point rotatePoint(const Point *target, const Vector *axis, float degrees);

#endif