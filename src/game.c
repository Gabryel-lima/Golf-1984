/********************************************************************
 * GOLF RETRÔ (inspirado no NES) — Versão Raylib em C99             *
 ********************************************************************/

#include "game.h"
#include <math.h>

// Globais
Player player;
Ball ball;
Course course;

void InitGame(void) {
    player.power = 0.0f;     // começamos sem carga
    player.isCharging = false;
    player.chargeTime = 0.0f;
    player.club = 1;
    player.position = (Vector2) {100, SCREEN_HEIGHT - 100};
    player.aimAngle = 0.0f;  // mira inicial: reto para a direita

    ball.position = player.position;
    ball.velocity = (Vector2) {0, 0};
    ball.inMotion = false;

    course.holePosition = (Vector2) {SCREEN_WIDTH - 100, player.position.y};
    course.windStrength = 0.3f;
    course.windAngle = 45.0f; // graus
}

void UpdateGame(void) {
    float dt = GetFrameTime();

    // Mira e disparo da bola
    if (!ball.inMotion) {
        // Ajuste de ângulo de mira (setas esquerda/direita)
        if (IsKeyDown(KEY_LEFT))  player.aimAngle += AIM_ROTATE_SPEED * dt;
        if (IsKeyDown(KEY_RIGHT)) player.aimAngle -= AIM_ROTATE_SPEED * dt;

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
            float angleRad = player.aimAngle * (PI / 180.0f); // mira do jogador, não o vento
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

        // Vento: tratado como aceleração contínua durante todo o voo
        // (decisão da Fase 0; revisar se a Fase 3 exigir outro modelo).
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

        // "Chão virtual" ao redor do buraco
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
    // player (marcador do tee)
    DrawCircleV(player.position, 10, BLUE);

    // linha de mira, só antes do tiro
    if (!ball.inMotion) {
        float angleRad = player.aimAngle * (PI / 180.0f);
        Vector2 aimEnd = {
            player.position.x + cosf(angleRad) * 40.0f,
            player.position.y - sinf(angleRad) * 40.0f
        };
        DrawLineV(player.position, aimEnd, YELLOW);
    }

    // hole
    DrawCircleV(course.holePosition, HOLE_RADIUS, BLACK);

    // ball
    DrawCircleV(ball.position, BALL_RADIUS, WHITE);

    // draw HUD
    DrawText("STROKE PLAY", 10, 10, 20, WHITE);
    DrawText(TextFormat("CLUB %dW", player.club), 10, 40, 20, WHITE);
    DrawText(TextFormat("POWER %.1f", player.power), 10, 70, 20, WHITE);
    DrawText(TextFormat("WIND %.1f @ %.0f°", course.windStrength, course.windAngle), 10, 100, 20, WHITE);
    DrawText(TextFormat("AIM %.0f°", player.aimAngle), 10, 130, 20, WHITE);
}
