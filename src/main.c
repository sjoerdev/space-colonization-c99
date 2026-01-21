#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

int main(void)
{
    int screenWidth = 800;
    int screenHeight = 450;

    SetTraceLogLevel(LOG_WARNING);

    InitWindow(screenWidth, screenHeight, "window");

    Camera3D camera = {
        .position = (Vector3){0.0f, 1.0f, 4.0f},
        .target = (Vector3){0.0f, 0.0f, 0.0f},
        .up = (Vector3){0.0f, 1.0f, 0.0f},
        .fovy = 60.0f,
        .projection = CAMERA_PERSPECTIVE
    };

    Vector3 cubePosition = {0.0f, 0.0f, 0.0f};

    while (!WindowShouldClose())
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);

                DrawGrid(10, 1.0f);

            EndMode3D();

            DrawFPS(10, 10);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}