/********************************************************************
 * GOLF RETRÔ (inspirado no NES) — Versão Raylib em C99             *
 * Compilação: gcc main.c -o golf -lraylib -lm                      *
 ********************************************************************/

#include "raylib.h"
#include <math.h>
#include <stdio.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 450

#define MAX_CHARGE_TIME 1.5f   // em segundos, atinge power = 1.0
#define MAX_SHOT_POWER 600.0f  // força máxima em pixels/s

#define BALL_RADIUS 5 // raio da
#define HOLE_RADIUS 8 // raio para “entrar” no buraco
#define HOLE_GROUND_RADIUS 120 // raio para “chão virtual” ao redor
// #define BALL_MASS 45.93f

// #define AIR_DRAG 0.5f // coeficiente de arrasto no ar (0.0–1.0)
// #define GROUND_FRICTION 0.4f // coeficiente de atrito ao rolar no solo

#define GRAVITY 9.80665f // m/s²
#define PPM 100.0f // pixels por metro
#define GRAVITY_PX (GRAVITY * PPM) // px/s²
// #define RESTITUTION_COEF 0.6f // 0 = absorve totalmente, 1 = ricochete perfeito

// Estrutura do jogador
typedef struct {
    Vector2 position;
    int club;      // 1W, 2W, 3W, etc.
    float power;   // 0.0 a 1.0
    bool  isCharging;    // indicará se estamos no “carregamento”
    float chargeTime;    // tempo acumulado de carregamento (s)
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
void DrawGame(void);

void InitGame(void) {
    player.power = 0.0f;     // começamos sem carga
    player.isCharging = false;
    player.chargeTime = 0.0f;
    player.club = 1;
    player.position = (Vector2) {100, SCREEN_HEIGHT - 100};

    ball.position = player.position;
    ball.velocity = (Vector2) {0, 0};
    ball.inMotion = false;

    course.holePosition = (Vector2) {SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100};
    course.windStrength = 0.3f;
    course.windAngle = 45.0f; // graus
}

void UpdateGame(void) {
    float dt = GetFrameTime();

    // Disparo da bola 
    if (!ball.inMotion) {
        // Carrega enquanto tecla estiver segurada
        if (IsKeyDown(KEY_SPACE)) {
            player.isCharging = true;
            player.chargeTime += dt;
            if (player.chargeTime > MAX_CHARGE_TIME)
                player.chargeTime = MAX_CHARGE_TIME;
            player.power = player.chargeTime / MAX_CHARGE_TIME; // para HUD
        }
        // Dispara ao soltar
        if (player.isCharging && IsKeyReleased(KEY_SPACE)) {
            float ratio = player.chargeTime / MAX_CHARGE_TIME;
            float angleRad = course.windAngle * (PI / 180.0f);    // use course.windAngle
            float shotVel = ratio * MAX_SHOT_POWER;
            ball.velocity = (Vector2){ cosf(angleRad) * shotVel, -sinf(angleRad) * shotVel };
            ball.inMotion = true;
            player.isCharging = false;
            player.chargeTime = 0;
        }
    }

    // Movimento
    if (ball.inMotion) {
        // Gravidade
        ball.velocity.y += GRAVITY_PX * dt;

        // Vento (usa course.windAngle corretamente)
        float windRad = course.windAngle * (PI / 180.0f);
        ball.velocity.x += course.windStrength * cosf(windRad) * dt;
        ball.velocity.y += course.windStrength * sinf(windRad) * dt;

        // Atualiza posição
        ball.position.x += ball.velocity.x * dt;
        ball.position.y += ball.velocity.y * dt;

        // colisões em todas as paredes
        float damp = 0.5f;

        float dx = ball.position.x - course.holePosition.x;
        float dy = ball.position.y - course.holePosition.y;
        float dist = sqrtf(dx*dx + dy*dy);
        if (dist < HOLE_RADIUS) {
            // entrou no buraco
            ball.position = course.holePosition;
            ball.velocity = (Vector2){ 0, 0 };
            ball.inMotion = false;
            return;
        }

        // “Chão virtual” ao redor do buraco
        if (dist < HOLE_GROUND_RADIUS) {
            // se estiver abaixo da altura do buraco, colide ali
            if (ball.position.y >= course.holePosition.y) {
                ball.position.y = course.holePosition.y;
                // quique suave
                ball.velocity.y = -ball.velocity.y * 0.5f;
                if (fabsf(ball.velocity.y) < 20.0f) {
                    ball.velocity = (Vector2){ 0, 0 };
                    ball.inMotion = false;
                }
                return;
            }
        }

        // esquerda
        if (ball.position.x <= BALL_RADIUS) {
            ball.position.x = BALL_RADIUS;
            ball.velocity.x = -ball.velocity.x * damp;
        }

        // direita
        if (ball.position.x >= SCREEN_WIDTH - BALL_RADIUS) {
            ball.position.x = SCREEN_WIDTH - BALL_RADIUS;
            ball.velocity.x = -ball.velocity.x * damp;
        }

        // teto
        if (ball.position.y <= BALL_RADIUS) {
            ball.position.y = BALL_RADIUS;
            ball.velocity.y = -ball.velocity.y * damp;
        }

        // chão
        if (ball.position.y >= SCREEN_HEIGHT - BALL_RADIUS) {
            ball.position.y = SCREEN_HEIGHT - BALL_RADIUS;
            ball.velocity.y = -ball.velocity.y * damp;
            if (fabsf(ball.velocity.y) < 3.0f) {
                ball.velocity = (Vector2){ 0, 0 };
                ball.inMotion = false;
            }
        }
    }
}

void DrawGame(void) {
    // player
    DrawCircleV(player.position, 10, BLUE);

    // hole
    DrawCircleV(course.holePosition, HOLE_RADIUS, BLACK);

    // ball
    DrawCircleV(ball.position, BALL_RADIUS, WHITE);

    // draw HUD
    DrawText("STROKE PLAY", 10, 10, 20, WHITE);
    DrawText(TextFormat("CLUB %dW", player.club), 10, 40, 20, WHITE);
    DrawText(TextFormat("POWER %.1f", player.power), 10, 70, 20, WHITE);
    DrawText(TextFormat("WIND %.1f @ %.0f°", course.windStrength, course.windAngle), 10, 100, 20, WHITE);
}

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
