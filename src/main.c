#include "camera.h"
#include "point.h"
#include "quaternion.h"

#include <SDL3/SDL.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

static int WINDOW_W = 800;
static int WINDOW_H = 600;

typedef struct {
    int start;
    int end;
} Edge;

Point projectPerspective(Point p, float f) {
    float w = -(p.z / f); 

    if (w != 0) {
        p.x /= w;
        p.y /= w;
        p.z /= w;
    }

    p.x *= 50;
    p.y *= 50;

    p.x += WINDOW_W / 2;
    p.y += WINDOW_H / 2;

    return p;
}

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

int main(int argc, char* argv[]) {

    if (!SDL_CreateWindowAndRenderer("3D Renderer", WINDOW_W, WINDOW_H, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return 1;
    }

    bool running = true;
    SDL_Event event;

    Point object_vertices[11] = {
        { 0.00, 3.00, 2.00 },     
        { 1.90, 3.00, 0.62 },     
        { 1.18, 3.00, -1.62 },    
        {-1.18, 3.00, -1.62 },    
        {-1.90, 3.00, 0.62 },     
        { 0.00, 1.75, 3.50 },     
        { 3.33, 1.75, 1.08 },     
        { 2.06, 1.75, -2.83 },    
        {-2.06, 1.75, -2.83 },    
        {-3.33, 1.75, 1.08 },     
        { 0.00, -2.50, 0.00 }      
    };

    Edge object_edges[20] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0},
        {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 5},
        {0, 5}, {1, 6}, {2, 7}, {3, 8}, {4, 9},
        {5, 10}, {6, 10}, {7, 10}, {8, 10}, {9, 10},
    };

    Camera camera;
    initCamera(&camera, (Point) {10, 5, 0}, (Point) {0, 0, 0}, (Vector) {0, 1, 0});

    const Uint8 *state;

    float object_angle = 0;

    float OBJECT_ROTAION_SPEED = 60;
    float CAMERA_MOVEMENT_SPEED = 20;
    float CAMERA_ROTATION_SPEED = 45;

    Uint64 last_time = SDL_GetTicks();

    bool relative_mouse_mode = true;
    SDL_SetWindowRelativeMouseMode(window, true);

    while (running) {
        Uint64 current_time = SDL_GetTicks();
        float delta_time = (current_time - last_time) / 1000.0f;
        last_time = current_time;

        object_angle += OBJECT_ROTAION_SPEED * delta_time;

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

        Point projected_points[11];

        for (int i = 0; i < 11; i++) {
            Point p = object_vertices[i];

            p = rotatePoint(p, (Vector) {0, 1, 0}, object_angle);
            p = multiplyMatrixPoint(camera.view_matrix, p);
            p = projectPerspective(p, 4);

            projected_points[i] = p;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for (int i = 0; i < 20; i++) {
            Point p1 = projected_points[object_edges[i].start];
            Point p2 = projected_points[object_edges[i].end];
            SDL_RenderLine(renderer, p1.x, p1.y, p2.x, p2.y);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
