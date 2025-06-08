#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "triangle.h"
#include "vector.h"

typedef struct {
    Point *vertices;
    int num_vertices;
    Triangle *faces;
    int num_faces;
    float angle_x;
    float angle_y;
    float angle_z;
} Mesh;

Mesh createMesh(Point *points, Triangle *faces, int num_points, int num_faces);

#endif