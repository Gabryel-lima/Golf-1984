/********************************************************************
 * GOLF RETRÔ (inspirado no NES) — Versão Raylib em C99             *
 * Compilação: gcc main.c -o golf -lraylib -lm                      *
 ********************************************************************/

#include "raylib.h"
#include <math.h>
#include <stdio.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 450

// Estrutura do jogador
typedef struct {
    Vector2 position;
    int club;      // 1W, 2W, 3W, etc.
    float power;   // 0.0 a 1.0
} Player;

// Estrutura da bola
typedef struct {
    Vector2 position;
    Vector2 velocity;
    bool inMotion;
} Ball;

// Estrutura do campo
typedef struct {
    Vector2 holePosition;
    float windStrength;
    float windAngle;
} Course;

// Globais
Player player;
Ball ball;
Course course;

// Funções principais
void InitGame(void);
// void UpdateGame(void);
// void DrawGame(void);

void InitGame(void) {
    player.club = 1;
    player.position = (Vector2) {100, 100 - SCREEN_WIDTH};
    player.power = 0.5f;

    ball.position = player.position;
    ball.velocity = (Vector2) {0, 0};
    ball.inMotion = false;

    course.holePosition = (Vector2) {SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100};
    course.windStrength = 0.3f;
    course.windAngle = 45.0f; // graus
}

// Entrypoint
int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Golf Retrô");
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
