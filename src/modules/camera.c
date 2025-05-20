#include "vector.h"
#include "utils.h"
#include "quaternion.h"
#include "camera.h"

#include <math.h>

void updateViewMatrix(Camera *camera) {
    float translation_matrix[4][4] = {
        {1, 0, 0, -camera->x},
        {0, 1, 0, -camera->y},
        {0, 0, 1, -camera->z},
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

void updateCoordinateSystem(Camera *camera, Vector normal) {
    camera->v = normalize(rejectFrom(camera->n, camera->up));
    camera->u = normalize(crossProduct(camera->n, camera->v));

    updateViewMatrix(camera);
}

void initCamera(Camera *camera, Point eye, Point at, Vector up) {
    camera->x = eye.x;
    camera->y = eye.y;
    camera->z = eye.z;

    camera->up = up;
    camera->n = normalize(createVector(eye, at));
    updateCoordinateSystem(camera, camera->n);
}

void moveCamera(Camera *camera, float dx, float dy, float dz) {
    if (dx) {
        camera->x += camera->u.x * dx;
        camera->z += camera->u.z * dx;
    }

    if (dy) {
        camera->y += camera->v.y * dy;
    }

    if (dz) {
        camera->x += camera->n.x * dz;
        camera->z += camera->n.z * dz;
    }

    updateViewMatrix(camera);
}

void rotateCamera(Camera *camera, float dx_degrees, float dy_degrees) {
    float x_radians = dx_degrees * (PI / 180);
    float y_radians = dy_degrees * (PI / 180);

    float x_rotation_matrix[4][4] = {
        {cos(x_radians), -sin(x_radians), 0, 0},
        {sin(x_radians), cos(x_radians), 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 0}
    };

    float y_rotation_matrix[4][4] = {
        {cos(y_radians), 0, sin(y_radians), 0},
        {0, 1, 0, 0},
        {-sin(y_radians), 0, cos(y_radians), 0},
        {0, 0, 0, 0}
    };

    if (dx_degrees) {
        camera->n = rotateVector(camera->n, camera->u, dx_degrees);
    }

    if (dy_degrees) {
        camera->n = multiplyMatrixVector(y_rotation_matrix, camera->n);
    }

    updateCoordinateSystem(camera, camera->n);
}

