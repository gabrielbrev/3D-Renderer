#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"
#include "mesh.h"
#include "light_source.h"

#include <SDL3/SDL.h>

extern const int CAMERA_MODE_FILL;
extern const int CAMERA_MODE_WIREFRAME;

typedef struct {
    Point pos;
    Vector n;
    Vector v;
    Vector u;
    Vector up;
    float view_matrix[4][4];
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Window *window;
    int width, height;
    int mode;
} Camera;

void updateViewMatrix(Camera *camera);
void updateCoordinateSystem(Camera *camera, const Vector *normal);
void initCamera(Camera *camera, SDL_Renderer *renderer, SDL_Window *window, const Point *eye, const Point *at, const Vector *up);
void cycleCameraMode(Camera *camera);
void moveCamera(Camera *camera, float dx, float dy, float dz);
void rotateCamera(Camera *camera, float dx_degrees, float dy_degrees);
void renderObjects(Camera *camera, MeshList *mesh_list, LightSource *light);

#endif