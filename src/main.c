#include "raylib.h"

int main(void) {
    InitWindow(800, 450, "Simple Window");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Welcome to the hacktoberfest challenge!!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

