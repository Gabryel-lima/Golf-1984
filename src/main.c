/********************************************************************
 * GOLF RETRÔ (inspirado no NES) — Versão Raylib em C99             *
 * Compilação: make                                                  *
 ********************************************************************/

#include "raylib.h"
#include "game.h"

// Entrypoint
int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Golf 1984");
    SetTargetFPS(60);
    InitGame();

    while (!WindowShouldClose()) {
        UpdateGame();
        BeginDrawing();
        ClearBackground(DARKGREEN);
        DrawGame();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
