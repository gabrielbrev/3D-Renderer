#include "mesh.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Mesh createMesh(Point position, Point *vertices, Triangle *faces, int num_vertices, int num_faces, Material material) {
    return (Mesh) { position, vertices, num_vertices, faces, num_faces, 0, 0, 0, material };
}

void readVertice(Point **p, float size, const Point *pos) {
    Point *point = malloc(sizeof(Point));
    if (point == NULL) {
        printf("Error allocating memory for vertice\n");
        *p = NULL;
        return;
    }

    for (int i = 0; i < 3; i++) {
        char *token = strtok(NULL, " ");
        if (token != NULL) {
            point->data[i] = atof(token) * size;
        } else {
            free(point);
            *p = NULL;
            printf("Error reading vertice\n");
            return;
        }
    }

    *p = point;
}

void readFace(Triangle **a, Triangle **b, const ColorHSV *color) {
    Triangle *face1 = malloc(sizeof(Triangle));
    Triangle *face2 = malloc(sizeof(Triangle));

    if (!face1 || !face2) {
        printf("Error allocating memory for face\n");
        return;
    }

    face1->color = *color;
    face2->color = *color;

    char *data[4];
    char *token;

    for (int i = 0; i < 4; i++) {
        token = strtok(NULL, " ");
        if (token != NULL) {
            data[i] = strdup(token);
        } else {
            data[i] = NULL;
        }
    }
    
    for (int i = 0; i < 4; i++) {
        if (data[i]) {
            char *vstr = strtok(data[i], "/");

            int value = vstr ? atoi(vstr) - 1 : -1;
            if (i < 3) face1->vertices[i] = value;
            if (i > 0) face2->vertices[i - 1] = value;

            free(data[i]);
        } else {
            if (i < 3) {
                *a = NULL;
                *b = NULL;
                free(face1);
                free(face2);
                printf("Error reading face\n");
                return;
            }
        }
    }

    *a = face1;
    if (data[3]) {
        face2->vertices[0] = face1->vertices[0];
        *b = face2;
    } else {
        *b = NULL;
        free(face2);
    }
}

void loadMesh(const char *filename, MeshList *mesh_list, Mesh **mesh, float size, Point pos, const ColorHSV *color) {
    FILE *file;
    char line[256];

    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    Point *vertices = NULL;
    Triangle *faces = NULL;
    int num_vertices = 0;
    int num_faces = 0;
    int vertice_capacity = 0;
    int face_capacity = 0;

    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, " ");
        
        if (token != NULL) {
            if (strcmp(token, "v") == 0) {
                if (num_vertices >= vertice_capacity) {
                    vertice_capacity = vertice_capacity ? vertice_capacity * 2 : 10;
                    vertices = realloc(vertices, vertice_capacity * sizeof(Point));
                    if (vertices == NULL) {
                        perror("Error allocating memory");
                        return;
                    }
                }


                Point *p = NULL;
                readVertice(&p, size, &pos);
                if (p) {
                    vertices[num_vertices++] = *p;
                    free(p);
                } else {
                    return;
                }
            } else if (strcmp(token, "f") == 0) {
                if (num_faces >= face_capacity) {
                    face_capacity = face_capacity ? face_capacity * 2 : 10;
                    faces = realloc(faces, face_capacity * sizeof(Triangle));
                    if (faces == NULL) {
                        perror("Error allocating memory");
                        return;
                    }
                }

                Triangle *face1 = NULL, *face2 = NULL;
                readFace(&face1, &face2, color);

                if (face1) {
                    faces[num_faces++] = *face1;
                    free(face1);
                } else {
                    return;
                }
                 
                if (face2) {
                    faces[num_faces++] = *face2;
                    free(face2);
                }
            }
        }
    }

    fclose(file);
        
    if (mesh_list->num_meshes >= mesh_list->mesh_capacity) {
        mesh_list->mesh_capacity = mesh_list->mesh_capacity ? mesh_list->mesh_capacity * 2 : 10;
        mesh_list->meshes = realloc(mesh_list->meshes, mesh_list->mesh_capacity * sizeof(Mesh));
        if (mesh_list->meshes == NULL) {
            perror("Error allocating memory");
            return;
        }
    }
    mesh_list->meshes[mesh_list->num_meshes++] = createMesh(pos, vertices, faces, num_vertices, num_faces, (Material) { 0.4f, 0.8f });
    if (mesh) *mesh = &mesh_list->meshes[mesh_list->num_meshes - 1];
}

void initMeshList(MeshList *mesh_list) {
    mesh_list->mesh_capacity = 0;
    mesh_list->num_meshes = 0;
    mesh_list->meshes = NULL;
}
