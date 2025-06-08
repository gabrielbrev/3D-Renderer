#include "camera.h"
#include "quaternion.h"
#include "mesh.h"
#include "triangle.h"

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

    Mesh obj = createMesh(
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
            {{0, 1, 2}, {0, 1, 1}},
            {{0, 2, 3}, {0, 1, 1}},
            {{0, 3, 4}, {0, 1, 1}},

            {{0, 5, 6}, {0, 1, 1}},
            {{0, 6, 1}, {0, 1, 1}},

            {{1, 6, 7}, {0, 1, 1}},
            {{1, 7, 2}, {0, 1, 1}},

            {{2, 7, 8}, {0, 1, 1}},
            {{2, 8, 3}, {0, 1, 1}},

            {{3, 8, 9}, {0, 1, 1}},
            {{3, 9, 4}, {0, 1, 1}},

            {{4, 9, 5}, {0, 1, 1}},
            {{4, 5, 0}, {0, 1, 1}},
            
            {{9, 10, 5}, {0, 1, 1}},

            {{5, 10, 6}, {0, 1, 1}},

            {{6, 10, 7}, {0, 1, 1}},

            {{7, 10, 8}, {0, 1, 1}},

            {{8, 10, 9}, {0, 1, 1}},
        },
        11, 18);

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
        if (state[SDL_SCANCODE_UP]) {
            rotateCamera(&camera, CAMERA_ROTATION_SPEED * delta_time, 0);
        }
        if (state[SDL_SCANCODE_DOWN]) {
            rotateCamera(&camera, -CAMERA_ROTATION_SPEED * delta_time, 0);
        }
        if (state[SDL_SCANCODE_LEFT]) {
            rotateCamera(&camera, 0, -CAMERA_ROTATION_SPEED * delta_time);
        }
        if (state[SDL_SCANCODE_RIGHT]) {
            rotateCamera(&camera, 0, CAMERA_ROTATION_SPEED * delta_time);
        }

        float dx, dy;
        SDL_GetRelativeMouseState(&dx, &dy);
        if (relative_mouse_mode && (dx || dy)) {
            rotateCamera(&camera, -dy, dx);
        }

        obj.angle_y += OBJECT_ROTAION_SPEED * delta_time;
        color_counter += COLOR_ROTATION_SPEED * delta_time;

        for (int i = 0; i < obj.num_faces; i++) {
            int modifier;
            if (i < 3) {
                modifier = 0;
            } else if (i < 13) {
                modifier = (i - 1) / 2;
            } else {
                modifier = i - 7;
            }

            Triangle *face = &obj.faces[i];
            face->color.h = 360 * (modifier + color_counter) / (obj.num_faces + 1);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        renderObject(&camera, &obj);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
