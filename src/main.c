#include "camera.h"
#include "quaternion.h"
#include "mesh.h"
#include "triangle.h"
#include "light_source.h"

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

    Mesh obj1 = createMesh(
        (Point[]) {
            {  0.00,  3.00,  2.00 },  
            {  1.90,  3.00,  0.62 },  
            {  1.18,  3.00, -1.62 },  
            { -1.18,  3.00, -1.62 },  
            { -1.90,  3.00,  0.62 },  
            {  0.00,  1.75,  3.50 },  
            {  3.33,  1.75,  1.08 },  
            {  2.06,  1.75, -2.83 },  
            { -2.06,  1.75, -2.83 },  
            { -3.33,  1.75,  1.08 },  
            {  0.00, -2.50,  0.00 }   
        },
        (Triangle[]) {
            {{0, 1, 2}, {170, 0.7, 1}},
            {{0, 2, 3}, {170, 0.7, 1}},
            {{0, 3, 4}, {170, 0.7, 1}},

            {{0, 5, 6}, {170, 0.7, 1}},
            {{0, 6, 1}, {170, 0.7, 1}},

            {{1, 6, 7}, {170, 0.7, 1}},
            {{1, 7, 2}, {170, 0.7, 1}},

            {{2, 7, 8}, {170, 0.7, 1}},
            {{2, 8, 3}, {170, 0.7, 1}},

            {{3, 8, 9}, {170, 0.7, 1}},
            {{3, 9, 4}, {170, 0.7, 1}},

            {{4, 9, 5}, {170, 0.7, 1}},
            {{4, 5, 0}, {170, 0.7, 1}},
            
            {{9, 10, 5}, {170, 0.7, 1}},

            {{5, 10, 6}, {170, 0.7, 1}},

            {{6, 10, 7}, {170, 0.7, 1}},

            {{7, 10, 8}, {170, 0.7, 1}},

            {{8, 10, 9}, {170, 0.7, 1}},
        },
        11, 18, (Material) {0.4, 0.8});

    Mesh obj2 = createMesh(
        (Point[]) {
            {  0.00 - 6,  3.00,  2.00 - 6 },  
            {  1.90 - 6,  3.00,  0.62 - 6 },  
            {  1.18 - 6,  3.00, -1.62 - 6 },  
            { -1.18 - 6,  3.00, -1.62 - 6 },  
            { -1.90 - 6,  3.00,  0.62 - 6 },  
            {  0.00 - 6,  1.75,  3.50 - 6 },  
            {  3.33 - 6,  1.75,  1.08 - 6 },  
            {  2.06 - 6,  1.75, -2.83 - 6 },  
            { -2.06 - 6,  1.75, -2.83 - 6 },  
            { -3.33 - 6,  1.75,  1.08 - 6 },  
            {  0.00 - 6, -2.50,  0.00 - 6 }   
        },
        (Triangle[]) {
            {{0, 1, 2}, {345, 0.9, 1}},
            {{0, 2, 3}, {345, 0.9, 1}},
            {{0, 3, 4}, {345, 0.9, 1}},

            {{0, 5, 6}, {345, 0.9, 1}},
            {{0, 6, 1}, {345, 0.9, 1}},

            {{1, 6, 7}, {345, 0.9, 1}},
            {{1, 7, 2}, {345, 0.9, 1}},

            {{2, 7, 8}, {345, 0.9, 1}},
            {{2, 8, 3}, {345, 0.9, 1}},

            {{3, 8, 9}, {345, 0.9, 1}},
            {{3, 9, 4}, {345, 0.9, 1}},

            {{4, 9, 5}, {345, 0.9, 1}},
            {{4, 5, 0}, {345, 0.9, 1}},
            
            {{9, 10, 5}, {345, 0.9, 1}},

            {{5, 10, 6}, {345, 0.9, 1}},

            {{6, 10, 7}, {345, 0.9, 1}},

            {{7, 10, 8}, {345, 0.9, 1}},

            {{8, 10, 9}, {345, 0.9, 1}},
        },
        11, 18, (Material) {0.4, 0.8});
        
    Mesh *meshes[] = { &obj1, &obj2 };

    LightSource light = { {5, 7, 3}, 1.0f, 0.3f, 0.001f, 0.001f, 1.0f };

    Camera camera;
    initCamera(&camera, renderer, window, &(Point) {10, 5, 0}, &(Point) {0, 0, 0}, &(Vector) {0, 1, 0});

    const Uint8 *state;

    float OBJECT_ROTAION_SPEED = 75;
    float CAMERA_MOVEMENT_SPEED = 20;
    float CAMERA_ROTATION_SPEED = 45;
    float COLOR_ROTATION_SPEED = 5;

    Uint64 last_time = SDL_GetTicks();

    bool relative_mouse_mode = true;
    SDL_SetWindowRelativeMouseMode(window, true);

    float color_counter = 0;

    while (running) {
        Uint64 current_time = SDL_GetTicks();
        float delta_time = (current_time - last_time) / 1000.0f;
        last_time = current_time;

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

        // obj1.angle_y += OBJECT_ROTAION_SPEED * delta_time;
        // obj2.angle_y -= OBJECT_ROTAION_SPEED * delta_time;

        light.pos.x = camera.pos.x;
        light.pos.y = camera.pos.y;
        light.pos.z = camera.pos.z;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        renderObject(&camera, meshes, 2, &light);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
