#include "mesh.h"
#include "surface.h"

#include <stdlib.h>
#include <math.h>

Mesh createMesh(
    Point *vertices, Triangle *faces, Surface *surfaces,
    int num_vertices, int num_faces, int num_surfaces,
    Material material
) {
    const int surface_resolution = 25;
    int verts_per_surface = (surface_resolution + 1) * (surface_resolution + 1);
    int faces_per_surface = surface_resolution * surface_resolution * 2;

    int total_vertices = num_vertices + num_surfaces * verts_per_surface;
    int total_faces = num_faces + num_surfaces * faces_per_surface;

    Point *all_vertices = malloc(sizeof(Point) * total_vertices);
    Triangle *all_faces = malloc(sizeof(Triangle) * total_faces);

    for (int i = 0; i < num_vertices; i++) {
        all_vertices[i] = vertices[i];
    }
    for (int i = 0; i < num_faces; i++) {
        all_faces[i] = faces[i];
    }

    int vertex_offset = num_vertices;
    int face_offset = num_faces;

    for (int s = 0; s < num_surfaces; s++) {
        Surface *surf = &surfaces[s];

        Point grid[surface_resolution+1][surface_resolution+1];

        for (int i = 0; i <= surface_resolution; i++) {
            float u = (float)i / surface_resolution; 
            for (int j = 0; j <= surface_resolution; j++) {
                float v = (float)j / surface_resolution;
                grid[i][j] = evaluateSurface(surf, u, v);
                all_vertices[vertex_offset + i*(surface_resolution+1) + j] = grid[i][j];
            }
        }

        for (int i = 0; i < surface_resolution; i++) {
            for (int j = 0; j < surface_resolution; j++) {
                int idx0 = vertex_offset + i * (surface_resolution+1) + j;
                int idx1 = vertex_offset + (i+1) * (surface_resolution+1) + j;
                int idx2 = vertex_offset + i * (surface_resolution+1) + (j+1);
                int idx3 = vertex_offset + (i+1) * (surface_resolution+1) + (j+1);

                all_faces[face_offset++] = (Triangle){{idx0, idx1, idx2}, surf->color};
                all_faces[face_offset++] = (Triangle){{idx1, idx3, idx2}, surf->color};
            }
        }

        vertex_offset += verts_per_surface;
    }

    // Retorna mesh com todos os dados
    Mesh mesh = {
        .vertices = all_vertices,
        .num_vertices = total_vertices,
        .faces = all_faces,
        .num_faces = total_faces,
        .material = material
    };

    return mesh;
}

