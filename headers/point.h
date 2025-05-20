#ifndef POINT_H
#define POINT_H

typedef union {
    struct { float x, y, z; };
    float data[3];
} Point;

Point multiplyMatrixPoint(float m[4][4], Point p);

#endif