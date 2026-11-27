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
#define DASH SPEED * 2.5
#define DASH_DURACION 0.15F
#define DASH_ESPERA 2
#define VEL_LANZA 5
#define LANZA_DURACION 5

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
    int monedas; // yolopusealvaro
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
// pos, rec, velX, velY, saltar, eje, tipo (1 = espada), arma, dir, vida, estado, invul, volar, sangre, monedas
Player jugador1 = {
    {X, Y},
    {X, Y, 64.0F, 64.0F},
    0,
    0,
    1,
    0,
    1,
    {},
    1,
    5,
    1,
    0,
    false,
    0,
    0};

// ataque de contacto de los enemigos
Armas contacto = {-1, {X, Y, 48.0F, 48.0F}, RED, 1, 0.0F};

// armas: tipo, rango, color, dano, tiempo entre ataques (seg)
Armas espada = {1, {X, Y, 16.0F, 32.0F}, RED, 2, 0.4F};
Armas lanza = {4, {X, Y, 32.0F, 16.0F}, BLUE, 8, 1.0F};

// pantalla 0
Objeto esc0[] = {
    {{0, 700, 2160, 200}, 1, BLACK},
    {{0, 400, 200, 200}, 1, BLUE},
    {{400, 400, 100, 100}, 1, RED},
    {{600, 200, 100, 100}, 1, RED}};

Enemigo ene0[] = {
    {{{800, 652}, {800, 652, 48.0F, 48.0F}, 0, 0, 0, 0, 0, {}, 1, 3}, 1, WHITE, 150.0F, 0.0F, 2.0F, contacto},
    {{{300, 652}, {300, 652, 48.0F, 48.0F}, 0, 0, 0, 0, 0, {}, 1, 5}, 1, GREEN, 200.0F, 0.0F, 3.0F, contacto}};

// pantalla 1
Objeto esc1[] = {
    {{0, 700, 2160, 200}, 1, BLACK},
    {{300, 500, 150, 50}, 1, BLUE}};

Enemigo ene1[] = {
    {{{900, 652}, {900, 652, 48.0F, 48.0F}, 0, 0, 0, 0, 0, {}, 1, 3}, 1, WHITE, 150.0F, 0.0F, 2.0F, contacto}};

// pantalla 2
Objeto esc2[] = {
    {{0, 700, 2160, 200}, 1, BLACK},
    {{500, 500, 200, 100}, 1, BLUE}};

Enemigo ene2[] = {
    {{{1000, 652}, {1000, 652, 48.0F, 48.0F}, 0, 0, 0, 0, 0, {}, 1, 3}, 1, GREEN, 150.0F, 0.0F, 2.0F, contacto}};

// los que usa el juego (cambian con cargar())
Objeto *escenario = esc0;
int numObjetos = sizeof(esc0) / sizeof(esc0[0]);
Enemigo *lista_enemigos = ene0;
int numEnemigos = sizeof(ene0) / sizeof(ene0[0]);