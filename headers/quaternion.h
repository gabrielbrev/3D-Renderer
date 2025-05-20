#ifndef QUATERNION_H
#define QUATERNION_H

#include "vector.h"
#include "point.h"
#include "utils.h"

typedef union {
    struct { float w, x, y, z; };
    float data[4];
} Quaternion;

Quaternion vecToQuaternion(Vector vector);
Quaternion pointToQuaternion(Point point);
Quaternion createRotationQuaternion(Vector vector, float radians);
Quaternion multiply(Quaternion a, Quaternion b);
Quaternion getConjugate(Quaternion quaternion);
Vector rotateVector(Vector target, Vector axis, float degrees);
Point rotatePoint(Point target, Vector axis, float degrees);

#endif