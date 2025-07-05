#include "surface.h"
#include "utils.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

Surface createSurface3x3(Point control_points[3][3], float weights[3][3], ColorHSV color) {
    Surface s;
    s.size = 3;
    s.color = color;

    s.control_points = malloc(3 * sizeof(Point *));
    s.weights = malloc(3 * sizeof(float *));
    for (int i = 0; i < 3; i++) {
        s.control_points[i] = malloc(3 * sizeof(Point));
        s.weights[i] = malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            s.control_points[i][j] = control_points[i][j];
            s.weights[i][j] = weights[i][j];
        }
    }

    return s;
}

Point evaluateSurface(const Surface *surf, float s, float t) {
    int n = surf->size - 1;
    float *Bs = malloc((n+1) * sizeof(float));
    float *Bt = malloc((n+1) * sizeof(float));

    for (int i = 0; i <= n; i++) {
        float binomial = (float)comb(n, i);
        Bs[i] = binomial * powf(s, i) * powf(1 - s, n - i);
        Bt[i] = binomial * powf(t, i) * powf(1 - t, n - i);
    }

    float x = 0, y = 0, z = 0;
    float denom = 0;

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            float b = surf->weights[i][j] * Bs[i] * Bt[j];
            x += b * surf->control_points[i][j].x;
            y += b * surf->control_points[i][j].y;
            z += b * surf->control_points[i][j].z;
            denom += b;
        }
    }

    free(Bs);
    free(Bt);

    return (Point){x / denom, y / denom, z / denom};
}
