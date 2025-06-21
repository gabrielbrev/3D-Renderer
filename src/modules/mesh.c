#include "mesh.h"

Mesh createMesh(Point *vertices, Triangle *faces, int num_vertices, int num_faces, Material material) {
    return (Mesh) { vertices, num_vertices, faces, num_faces, 0, 0, 0, material };
}
