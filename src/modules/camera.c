#include "vector.h"
#include "utils.h"
#include "quaternion.h"
#include "camera.h"
#include "mesh.h"
#include "color.h"
#include "light_source.h"

#include <math.h>

const int CAMERA_MODE_FILL = 0;
const int CAMERA_MODE_WIREFRAME = 1;

void updateViewMatrix(Camera *camera) {
    float translation_matrix[4][4] = {
        {1, 0, 0, -camera->pos.x},
        {0, 1, 0, -camera->pos.y},
        {0, 0, 1, -camera->pos.z},
        {0, 0, 0, 1}
    };

    float rotation_matrix[4][4] ={
        {camera->u.x, camera->u.y, camera->u.z, 0},
        {camera->v.x, camera->v.y, camera->v.z, 0},
        {camera->n.x, camera->n.y, camera->n.z, 0},
        {0, 0, 0, 1}
    };

    multiplyMatrices4x4(camera->view_matrix, rotation_matrix, translation_matrix);
}

void updateCoordinateSystem(Camera *camera, const Vector *normal) {
    camera->v = rejectFrom(normal, &camera->up);
    camera->v = normalize(&camera->v);
    
    camera->u = crossProduct(normal, &camera->v);
    camera->u = normalize(&camera->u);
    
    updateViewMatrix(camera);
}

void initCamera(Camera *camera, SDL_Renderer *renderer, SDL_Window *window, const Point *eye, const Point *at, const Vector *up) {
    SDL_GetWindowSize(window, &camera->width, &camera->height);
    camera->window = window;
    camera->renderer = renderer;
    camera->texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        camera->width,
        camera->height
    );

    camera->mode = CAMERA_MODE_FILL;

    if (!camera->texture) {
        printf("Erro ao criar textura: %s\n", SDL_GetError());
        return;
    }
    camera->pos.x = eye->x;
    camera->pos.y = eye->y;
    camera->pos.z = eye->z;

    camera->up = *up;
    Vector n = createVector(eye, at);
    camera->n = normalize(&n);

    updateCoordinateSystem(camera, &camera->n);
}

void cycleCameraMode(Camera *camera) {
    camera->mode = (camera->mode + 1) % 2;
}


void moveCamera(Camera *camera, float dx, float dy, float dz) {
    if (dx) {
        camera->pos.x += camera->u.x * dx;
        camera->pos.z += camera->u.z * dx;
    }

    if (dy) {
        camera->pos.y += camera->v.y * dy;
    }

    if (dz) {
        camera->pos.x += camera->n.x * dz;
        camera->pos.z += camera->n.z * dz;
    }

    updateViewMatrix(camera);
}

void rotateCamera(Camera *camera, float dx_degrees, float dy_degrees) {
    float x_radians = dx_degrees * (PI / 180);
    float y_radians = dy_degrees * (PI / 180);

    if (dx_degrees) {
        camera->n = rotateVector(&camera->n, &camera->u, dx_degrees);
    }

    if (dy_degrees) {
        camera->n = rotateVector(&camera->n, &camera->up, dy_degrees);
    }

    updateCoordinateSystem(camera, &camera->n);
}

Point projectPerspective(const Point *p, float f) {
    Point proj = *p;
    float w = -(proj.z / f); 

    if (w != 0) {
        proj.x /= w;
        proj.y /= w;
    }

    proj.x *= 50;
    proj.y *= 50;

    proj.x += 800 / 2;
    proj.y += 600 / 2;

    return proj;
}

bool renderFace(Camera *camera, const Point *a, const Point *b, const Point *c) {
    Point centroid = getCentroid(a, b, c);
    Vector normal = getNormal(a, b, c);

    Vector direction = createVector(&camera->pos, &centroid);
    direction = normalize(&direction);

    float cos_theta = dot(&direction, &normal);
    return (cos_theta > 0);
}

void renderObjects(Camera *camera, MeshList *mesh_list, LightSource *light) {
    float zbuffer[camera->width][camera->height];
    for (int i = 0; i < camera->width; i++) {
        for (int j = 0; j < camera->height; j++) {
            zbuffer[i][j] = INFINITY;
        }
    }

    void* pixels = NULL;
    int pitch = 0;
    SDL_LockTexture(camera->texture, NULL, &pixels, &pitch);
    Uint32* pixel_data = (Uint32*)pixels;

    for (int mesh_idx = 0; mesh_idx < mesh_list->num_meshes; mesh_idx++) {
        Mesh *mesh = &mesh_list->meshes[mesh_idx];
        
        Point transformed_points[mesh->num_vertices];
        Point projected_points[mesh->num_vertices];
        
        for (int i = 0; i < mesh->num_vertices; i++) {
            Point p = mesh->vertices[i];

            p = rotatePoint(&p, &(Vector){1, 0, 0}, mesh->angle_x);
            p = rotatePoint(&p, &(Vector){0, 1, 0}, mesh->angle_y);
            p = rotatePoint(&p, &(Vector){0, 0, 1}, mesh->angle_z);

            p.x += mesh->pos.x;
            p.y += mesh->pos.y;
            p.z += mesh->pos.z;
            
            transformed_points[i] = p;
            
            p = multiplyMatrixVector(camera->view_matrix, &p);
            p = projectPerspective(&p, 4);
            projected_points[i] = p;
        }
        
        for (int i = 0; i < mesh->num_faces; i++) {
            Triangle face = mesh->faces[i];
            
            switch (camera->mode) {
                case CAMERA_MODE_FILL:
                Point *a = &projected_points[face.vertices[0]];
                Point *b = &projected_points[face.vertices[1]];
                Point *c = &projected_points[face.vertices[2]];

                if (!renderFace(
                    camera, 
                    &transformed_points[face.vertices[0]], 
                    &transformed_points[face.vertices[1]], 
                    &transformed_points[face.vertices[2]]
                )) continue;
                
                float light_intensity = getFaceLightIntensity(
                    light, 
                    &transformed_points[face.vertices[0]], 
                    &transformed_points[face.vertices[1]], 
                    &transformed_points[face.vertices[2]], 
                    &mesh->material
                );
                
                // Rasterização
                SDL_Rect boundingRect = getBoundingRect(a, b, c);
                int minX = maxInt(0, boundingRect.x - 1);
                int minY = maxInt(0, boundingRect.y - 1);
                int maxX = minInt(camera->width, boundingRect.x + boundingRect.w + 1);
                int maxY = minInt(camera->height, boundingRect.y + boundingRect.h + 1);
                Uint32 color = HSVtoUint32(&face.color, light_intensity);
                
                for (int x = minX; x < maxX; x++) {
                    for (int y = minY; y < maxY; y++) {
                        Point p = {x, y};
                        Vector v0 = subtract(b, a);
                        Vector v1 = subtract(c, a);
                        Vector v2 = subtract(&p, a);
                        
                        float d00 = dot2D(&v0, &v0);
                        float d01 = dot2D(&v0, &v1);
                        float d11 = dot2D(&v1, &v1);
                        float d20 = dot2D(&v2, &v0);
                        float d21 = dot2D(&v2, &v1);
                        
                        float denom = d00 * d11 - d01 * d01;
                        float v = (d11 * d20 - d01 * d21) / denom;
                        float w = (d00 * d21 - d01 * d20) / denom;
                        float u = 1 - v - w;
                        
                        if (v >= 0 && w >= 0 && u >= 0) {
                            float z = u * a->z + v * b->z + w * c->z;
                            
                            if (x >= 0 && x < camera->width && y >= 0 && y < camera->height && z < zbuffer[x][y]) {
                                zbuffer[x][y] = z;
                                pixel_data[y * (pitch / 4) + x] = color;
                            }
                        }
                    }
                }
                
                break;
                
                case CAMERA_MODE_WIREFRAME:    
                for (int j = 0; j < 3; j++) {
                    Point p1 = projected_points[face.vertices[j]];
                    Point p2 = projected_points[face.vertices[(j + 1) % 3]];
                    SDL_RenderLine(camera->renderer, p1.x, p1.y, p2.x, p2.y);
                }  
                
                break;
            }    
        }
    }
    
    SDL_UnlockTexture(camera->texture);
    SDL_RenderTexture(camera->renderer, camera->texture, NULL, NULL);
}

