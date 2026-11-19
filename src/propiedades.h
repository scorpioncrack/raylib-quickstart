#pragma once
#include "raylib.h"
#include "resource_dir.h"
#define X 510
#define Y 400
#define SPEED 5
#define SALTO 10
#define GRAVEDAD 0.4F
#define IFRAME 1.0F
#define FRAM 4 
#define DASH SPEED*2.5
#define DASH_DURACION  0.15F
#define DASH_ESPERA 2
typedef struct Armas
{
    int tipo;
    Rectangle rango;
    Color color;
    int dano;
    float tiempo;
} _armas;

typedef struct Player
{
    Vector2 pos;
    Rectangle rec;
    float velX;
    float velY;
    int saltar;
    int eje;
    int tipo;
    Armas arma;
    int dir;
    int vida;
    int estado;
    float invul;
    bool volar;
    float sangre;
} _player;

typedef struct Objeto
{
    Rectangle rect;
    int blocking;
    Color color;
} _objeto;

typedef struct Enemigo
{
    Player base;
    int estado;
    Color color;
    float distancia;
    float desplazado;
    float velocidad;
    Armas ataque;
} _enemigo;

// jugador
Player jugador1 = {
    {X, Y},               // pos
    {X, Y, 64.0F, 64.0F}, // rec
    0,                    // velX
    0,                    // velY
    1,                    // saltar
    0,                    // eje
    1,                    // tipo (arma: 1 = espada)
    {},                   // arma (la llena el switch del main)
    1,                    // dir (1 = derecha)
    5,                    // vida
    1,                    // estado
    0,                    // invul
    false,                // volar
    0                     //sangre
};

// ataques enemigos: tipo, rango, color, dano, tiempo
Armas contacto = {-1, {X, Y, 32.0F, 32.0F}, RED, 1, 0.0F};

// LISTA DE ENEMIGOS
// base (Player): pos, rec, velX, velY, saltar, eje, tipo, arma, dir, vida
// resto: estado (1 vivo, 0 muerto), color, distancia, desplazado, velocidad, ataque
Enemigo lista_enemigos[] = {
    {{{800, 668}, {800, 668, 32.0F, 32.0F}, 0, 0, 0, 0, 0, {}, 1, 3}, 1, WHITE, 150.0F, 0.0F, 2.0F, contacto},
    {{{300, 668}, {300, 668, 32.0F, 32.0F}, 0, 0, 0, 0, 0, {}, 1, 5}, 1, GREEN, 200.0F, 0.0F, 3.0F, contacto}};

int numEnemigos = sizeof(lista_enemigos) / sizeof(lista_enemigos[0]);

// armas: tipo, rango, color, dano, tiempo entre ataques (seg)
Armas espada = {1, {X, Y, 16.0F, 32.0F}, RED, 2, 0.4F};

// objetos con colision
Objeto escenario[] = {
    {{0, 700, 1020, 200}, 1, BLACK}, // suelo
    {{0, 400, 200, 200}, 1, BLUE},   // plataforma 1
    {{400, 400, 100, 100}, 1, RED},  // plataforma 2
    {{600, 200, 100, 100}, 1, RED}   // plataforma 3
};
int numObjetos = sizeof(escenario) / sizeof(escenario[0]);

// AREA
Objeto area1 = {{0, 0, 1080, 1020}, 0, WHITE};
