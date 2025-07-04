#include "quaternion.h"
#include "vector.h"
#include "utils.h"

#include <math.h>

Quaternion vecToQuaternion(const Vector* vector) {
    return (Quaternion) {0, vector->x, vector->y, vector->z}; 
}

Quaternion pointToQuaternion(const Point* point) {
    return (Quaternion) {0, point->x, point->y, point->z}; 
}

Quaternion createRotationQuaternion(const Vector* vector, float radians) {
    Vector normalized = normalize(vector);
    float sin_radians = sinf(radians / 2);
    return (Quaternion) {cosf(radians / 2), sin_radians * normalized.x, sin_radians * normalized.y, sin_radians * normalized.z};
}

Quaternion multiply(const Quaternion* a, const Quaternion* b) {
    Quaternion result;
    result.w = a->w * b->w - a->x * b->x - a->y * b->y - a->z * b->z;
    result.x = a->w * b->x + a->x * b->w + a->y * b->z - a->z * b->y;
    result.y = a->w * b->y - a->x * b->z + a->y * b->w + a->z * b->x;
    result.z = a->w * b->z + a->x * b->y - a->y * b->x + a->z * b->w;
    return result;
}

Quaternion getConjugate(const Quaternion* quaternion) {
    return (Quaternion) {quaternion->w, -quaternion->x, -quaternion->y, -quaternion->z};
}

Vector rotateVector(const Vector* target, const Vector* axis, float degrees) {
    float radians = degrees * PI / 180.0f;
    Quaternion target_quaternion = vecToQuaternion(target);
    Quaternion rotation_quaternion = createRotationQuaternion(axis, radians);
    Quaternion rotation_conjugate = getConjugate(&rotation_quaternion);
    Quaternion result = multiply(
        &rotation_quaternion,
        &target_quaternion
    );
    result = multiply(&result, &rotation_conjugate);
    return (Vector) {result.x, result.y, result.z};
}

Point rotatePoint(const Point* target, const Vector* axis, float degrees) {
    float radians = degrees * PI / 180.0f;
    Quaternion target_quaternion = pointToQuaternion(target);
    Quaternion rotation_quaternion = createRotationQuaternion(axis, radians);
    Quaternion rotation_conjugate = getConjugate(&rotation_quaternion);
    Quaternion result = multiply(
        &rotation_quaternion,
        &target_quaternion
    );
    result = multiply(&result, &rotation_conjugate);
    return (Point) {result.x, result.y, result.z};
}