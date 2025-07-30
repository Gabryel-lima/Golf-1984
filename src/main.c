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
void UpdateGame(void);
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

void UpdateGame(void) {
    if(!ball.inMotion && IsKeyPressed(KEY_SPACE)) {
        float angle = 45.0f * (PI / 180.0f); // 45° em radianos
        float power = player.power * 600.0f;
        ball.velocity = (Vector2) { cosf(angle) * power -sinf(angle) * power };
        ball.inMotion = true;
    }

    if(ball.inMotion) {
        ball.velocity.y += 9.8f * 0.5f; // gravidade
        ball.position.x += ball.velocity.x * GetFrameTime();
        ball.velocity.y += ball.velocity.y * GetFrameTime();
        
        // limite do chão
        if(ball.position.y > SCREEN_HEIGHT - 10) {
            ball.position.y = SCREEN_HEIGHT - 10;
            ball.velocity = (Vector2) {0, 0};
            ball.inMotion = false;
        }
    }
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
