// basic_window.c
#include "raylib.h"

int main() {
    InitWindow(800, 450, "raylib [core] example - basic window");
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Congrats! raylib works on Ubuntu!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
