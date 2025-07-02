#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "triangle.h"
#include "vector.h"

typedef struct {
    float ambient_reflectivity;
    float diffuse_reflectivity;
} Material;

typedef struct {
    Point pos;
    Point *vertices;
    int num_vertices;
    Triangle *faces;
    int num_faces;
    float angle_x;
    float angle_y;
    float angle_z;
    Material material;
} Mesh;

typedef struct {
    int num_meshes;
    int mesh_capacity;
    Mesh *meshes;
} MeshList;

Mesh createMesh(Point position, Point *points, Triangle *faces, int num_points, int num_faces, Material material);
void loadMesh(const char *filename, MeshList *mesh_list, Mesh **mesh, float size, Point pos, const ColorHSV *color);
void initMeshList(MeshList *mesh_list);

#endif