#include "light_source.h"
#include "vector.h"

#include <math.h>

float getAtenuationFactor(const LightSource *light, float distance) {
    float a = light->attenuationQuadratic;
    float b = light->attenuationLinear;
    float c = light->attenuationConstant;
    float d = distance;

    return 1.0f / (a * d * d + b * d + c);
}

float getFaceLightIntensity(const LightSource *light, const Point *a, const Point *b, const Point *c, Material *material) {
    Point centroid = getCentroid(a, b, c);
    Vector normal = getNormal(a, b, c);

    Vector direction = createVector(&light->pos, &centroid);
    float distance = magnitude(&direction);
    direction = normalize(&direction);

    float cos_theta = dot(&direction, &normal);
    if (cos_theta < 0.0f) cos_theta = 0.0f;

    float fatt = getAtenuationFactor(light, distance);

    float Ia = light->ambient_intensity, ra = material->ambient_reflectivity;
    float Id = light->diffuse_intensity, rd = material->diffuse_reflectivity;

    float intensity = Ia * ra + fatt * Id * rd * cos_theta;
    if (intensity > 1.0f) intensity = 1.0f;

    return intensity;
}
