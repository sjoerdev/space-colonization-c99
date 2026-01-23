#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "simulation.h"

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

    InitializeSimulation();

    while (!WindowShouldClose())
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) UpdateCamera(&camera, CAMERA_THIRD_PERSON);

        if (IsKeyPressed(KEY_F)) IterateSpaceColonization();
        
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawGrid(10, 1.0f);

                // draw lines
                for (int i = 0; i < list_size(&lines); i++)
                {
                    Line* line = list_get(&lines, i);
                    DrawLine3D(line->start, line->end, BLUE);
                }

            EndMode3D();

            // draw nodes
            for (int i = 0; i < list_size(&nodes); i++)
            {
                Vector3* node = list_get(&nodes, i);
                Vector2 screenSpace = GetWorldToScreen(*node, camera);
                DrawCircleV(screenSpace, 2, RED);
            }

            DrawFPS(10, 10);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}