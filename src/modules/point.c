#include "point.h"

Point multiplyMatrixPoint(float m[4][4], Point p) {
    Point result;
    float x = p.x, y = p.y, z = p.z;
    float w = 1;

    result.x = m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3] * w;
    result.y = m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3] * w;
    result.z = m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3] * w;
    w = m[3][0] * x + m[3][1] * y + m[3][2] * z + m[3][3] * w;

    return result;
}