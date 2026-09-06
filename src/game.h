#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include <stdbool.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 450

#define MAX_CHARGE_TIME 1.5f   // em segundos, atinge power = 1.0
#define MAX_SHOT_POWER 600.0f  // força máxima em pixels/s

#define BALL_RADIUS 5           // raio da bola
#define HOLE_RADIUS 8           // raio para "entrar" no buraco
#define HOLE_GROUND_RADIUS 120  // raio para "chão virtual" ao redor do buraco

#define GRAVITY 9.80665f // m/s²
#define PPM 100.0f // pixels por metro
#define GRAVITY_PX (GRAVITY * PPM) // px/s²

#define AIM_ROTATE_SPEED 90.0f // graus/s de ajuste de mira (setas esquerda/direita)

// Estrutura do jogador
typedef struct {
    Vector2 position;
    int club;          // 1W, 2W, 3W, etc.
    float power;       // 0.0 a 1.0
    bool  isCharging;  // indica se estamos no "carregamento"
    float chargeTime;  // tempo acumulado de carregamento (s)
    float aimAngle;    // graus, 0 = direita; mira controlada pelo jogador
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

extern Player player;
extern Ball ball;
extern Course course;

void InitGame(void);
void UpdateGame(void);
void DrawGame(void);

#endif // GAME_H
