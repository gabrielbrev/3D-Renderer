#include "light_source.h"
#include "vector.h"
#include "color.h"

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

float getPhongLightIntensity(const LightSource *light, const Point *a, const Point *b, const Point *c, const Point *camera_pos, Material *material) {
    Point centroid = getCentroid(a, b, c);
    Vector normal = getNormal(a, b, c);

    Vector light_dir = createVector(&centroid, &light->pos);
    float distance = magnitude(&light_dir);
    light_dir = normalize(&light_dir);

    Vector view_dir = createVector(&centroid, camera_pos);
    view_dir = normalize(&view_dir);

    Vector incident_light = scalarMultiply(-1.0f, &light_dir);
    Vector reflection = reflect(&incident_light, &normal);

    float ambient = light->ambient_intensity * material->ambient_reflectivity;

    float cos_theta = dot(&light_dir, &normal);
    if (cos_theta < 0.0f) cos_theta = 0.0f;
    
    float fatt = getAtenuationFactor(light, distance);
    float diffuse = fatt * light->diffuse_intensity * material->diffuse_reflectivity * cos_theta;

    float specular = 0.0f;
    if (cos_theta > 0.0f) {
        float cos_alpha = dot(&reflection, &view_dir);
        if (cos_alpha > 0.0f) {
            specular = fatt * light->specular_intensity * material->specular_reflectivity * 
                      powf(cos_alpha, material->shininess);
        }
    }

    float intensity = ambient + diffuse + specular;
    if (intensity > 1.0f) intensity = 1.0f;

    return intensity;
}

ColorRGB getPhongColor(const LightSource *light, const Point *a, const Point *b, const Point *c, const Point *camera_pos, Material *material, const ColorHSV *surface_color) {
    Point centroid = getCentroid(a, b, c);
    Vector normal = getNormal(a, b, c);

    Vector light_dir = createVector(&centroid, &light->pos);
    float distance = magnitude(&light_dir);
    light_dir = normalize(&light_dir);

    Vector view_dir = createVector(&centroid, camera_pos);
    view_dir = normalize(&view_dir);

    Vector incident_light = scalarMultiply(-1.0f, &light_dir);
    Vector reflection = reflect(&incident_light, &normal);

    ColorRGB surface_rgb = HSVtoRGBFloat(surface_color, 1.0f);

    float ambient_r = light->ambient_intensity * material->ambient_reflectivity * surface_rgb.r;
    float ambient_g = light->ambient_intensity * material->ambient_reflectivity * surface_rgb.g;
    float ambient_b = light->ambient_intensity * material->ambient_reflectivity * surface_rgb.b;

    float cos_theta = dot(&light_dir, &normal);
    if (cos_theta < 0.0f) cos_theta = 0.0f;
    
    float fatt = getAtenuationFactor(light, distance);
    float diffuse_factor = fatt * light->diffuse_intensity * material->diffuse_reflectivity * cos_theta;
    float diffuse_r = diffuse_factor * light->color.r * surface_rgb.r;
    float diffuse_g = diffuse_factor * light->color.g * surface_rgb.g;
    float diffuse_b = diffuse_factor * light->color.b * surface_rgb.b;

    float specular_r = 0.0f, specular_g = 0.0f, specular_b = 0.0f;
    if (cos_theta > 0.0f) {
        float cos_alpha = dot(&reflection, &view_dir);
        if (cos_alpha > 0.0f) {
            float specular_factor = fatt * light->specular_intensity * material->specular_reflectivity * 
                                  powf(cos_alpha, material->shininess);
            specular_r = specular_factor * light->color.r;
            specular_g = specular_factor * light->color.g;
            specular_b = specular_factor * light->color.b;
        }
    }

    ColorRGB result;
    result.r = ambient_r + diffuse_r + specular_r;
    result.g = ambient_g + diffuse_g + specular_g;
    result.b = ambient_b + diffuse_b + specular_b;

    if (result.r > 1.0f) result.r = 1.0f;
    if (result.g > 1.0f) result.g = 1.0f;
    if (result.b > 1.0f) result.b = 1.0f;

    return result;
}
