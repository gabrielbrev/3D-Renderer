#include "camera.h"
#include "quaternion.h"
#include "mesh.h"
#include "triangle.h"
#include "light_source.h"
#include "color.h"

#include <SDL3/SDL.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

static int WINDOW_W = 800;
static int WINDOW_H = 600;

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

int main(int argc, char* argv[]) {

    if (!SDL_CreateWindowAndRenderer("3D Renderer", WINDOW_W, WINDOW_H, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return 1;
    }

    bool running = true;
    SDL_Event event;

    ColorHSV color1 = {345, 0.9, 1};
    ColorHSV color2 = {0, 0, 0.75};

    MeshList mesh_list;
    initMeshList(&mesh_list);

    for (int i = 0; i < 7; i++) {
        loadMesh("assets/diamond.obj", &mesh_list, NULL, 0.75, (Point) {8 * i, 0, 0}, &(ColorHSV) {360 / (i + 1), 0.9, 1});
    }

    LightSource light = { {5, 7, 3}, 1.0f, 0.3f, 0.001f, 0.001f, 1.0f };

    Camera camera;
    initCamera(&camera, renderer, window, &(Point) {10, 5, 5}, &(Point) {0, 0, 0}, &(Vector) {0, 1, 0});

    const Uint8 *state;

    float OBJECT_ROTAION_SPEED = 75;
    float CAMERA_MOVEMENT_SPEED = 20;
    float CAMERA_ROTATION_SPEED = 45;
    float COLOR_ROTATION_SPEED = 5;

    Uint64 last_time = SDL_GetTicks();

    bool relative_mouse_mode = true;
    SDL_SetWindowRelativeMouseMode(window, true);

    float counter = 0;

    while (running) {
        Uint64 current_time = SDL_GetTicks();
        float delta_time = (current_time - last_time) / 1000.0f;
        last_time = current_time;
        counter += 1 * delta_time;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }

            if (event.type == SDL_EVENT_WINDOW_FOCUS_GAINED) {
                relative_mouse_mode = true;
                SDL_SetWindowRelativeMouseMode(window, true);
            }

            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE) {
                    relative_mouse_mode = false;
                    SDL_SetWindowRelativeMouseMode(window, false);
                }
                if (event.key.key == SDLK_M) {
                    cycleCameraMode(&camera);
                }
            }

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (!relative_mouse_mode) {
                        relative_mouse_mode = true;
                        SDL_SetWindowRelativeMouseMode(window, true);
                    }
                }
            }
        }

        const bool *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_W]) {
            moveCamera(&camera, 0, 0, CAMERA_MOVEMENT_SPEED * delta_time);
        }
        if (state[SDL_SCANCODE_S]) {
            moveCamera(&camera, 0, 0, -CAMERA_MOVEMENT_SPEED * delta_time);
        }
        if (state[SDL_SCANCODE_A]) {
            moveCamera(&camera, CAMERA_MOVEMENT_SPEED * delta_time, 0, 0);
        }
        if (state[SDL_SCANCODE_D]) {
            moveCamera(&camera, -CAMERA_MOVEMENT_SPEED * delta_time, 0, 0);
        }
        if (state[SDL_SCANCODE_SPACE]) {
            moveCamera(&camera, 0, CAMERA_MOVEMENT_SPEED * delta_time, 0);
        }
        if (state[SDL_SCANCODE_LSHIFT]) {
            moveCamera(&camera, 0, -CAMERA_MOVEMENT_SPEED * delta_time, 0);
        }

        float dx, dy;
        SDL_GetRelativeMouseState(&dx, &dy);
        if (relative_mouse_mode && (dx || dy)) {
            rotateCamera(&camera, -dy, dx);
        }

        for (int i = 0; i < mesh_list.num_meshes; i++) {
            Mesh *mesh = &mesh_list.meshes[i];
            mesh->angle_y -= OBJECT_ROTAION_SPEED * delta_time;

            float angle = ((2.0f * PI) / ((float)mesh_list.num_meshes / (float)(i + 1))) + counter;

            mesh->pos.x = sinf(angle) * 8;
            mesh->pos.z = cosf(angle) * 8;
        }

        light.pos.x = camera.pos.x;
        light.pos.y = camera.pos.y;
        light.pos.z = camera.pos.z;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        renderObjects(&camera, &mesh_list, &light);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
