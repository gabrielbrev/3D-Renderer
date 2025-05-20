#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"
#include "point.h"

typedef struct {
    float x, y, z;
    Vector n;
    Vector v;
    Vector u;
    Vector up;
    float view_matrix[4][4];
} Camera;

void updateViewMatrix(Camera *camera);
void updateCoordinateSystem(Camera *camera, Vector normal);
void initCamera(Camera *camera, Point eye, Point at, Vector up);
void moveCamera(Camera *camera, float dx, float dy, float dz);
void rotateCamera(Camera *camera, float dx_degrees, float dy_degrees);

#endif