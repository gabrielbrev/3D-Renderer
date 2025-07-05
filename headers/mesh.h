#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "triangle.h"
#include "vector.h"
#include "surface.h"

typedef struct {
    float ambient_reflectivity;
    float diffuse_reflectivity;
} Material;

typedef struct {
    Point *vertices;
    int num_vertices;
    Triangle *faces;
    int num_faces;
    float angle_x;
    float angle_y;
    float angle_z;
    Material material;
} Mesh;

Mesh createMesh(Point *points, Triangle *faces, Surface *surfaces, int num_points, int num_faces, int num_surfaces, Material material);

#endif