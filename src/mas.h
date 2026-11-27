// TIENDA (libreria autocontenida)
#pragma once
#include "propiedades.h"

// ---------- configuracion ----------
#define PANTALLA_TIENDA 0 // pantalla donde aparece (0 = la de inicio)

enum { HAB_DASH = 0, HAB_VOLAR, HAB_SANGRE, HAB_CURAR, HAB_FUERZA, NUM_HAB };

typedef struct Habilidad
{
    const char *nombre;
    const char *descripcion;
    int precio;
    bool comprada;
} _habilidad;

// ---------- datos ----------
static Habilidad habilidades[NUM_HAB] = {
    {"Dash",            "Impulso rapido hacia adelante (C)",       10, false},
    {"Murcielago",      "Transformate y vuela (A)",                20, false},
    {"Lanza sangre",    "Dispara un proyectil de sangre (F)",      30, false},
    {"Curacion",        "Recupera 3 de vida, cuesta 2 sangre (S)", 40, false},
    {"Fortalecimiento", "Mas dano 15 seg, cuesta 1 sangre (D)",    50, false}};

// parada sobre el suelo (suelo en y = 700, 572 + 128 = 700)
static Rectangle tiendaIcono = {700, 572, 128, 128};
static Texture2D tiendaTex = {0};
static bool tiendaAbierta = false;
static int tiendaSel = 0;

// ---------- carga / descarga ----------
static void tienda_cargar()
{
    tiendaTex = LoadTexture("tienda.png"); // si no existe, se dibuja un cuadro dorado
}

static void tienda_descargar()
{
    if (tiendaTex.id != 0)
        UnloadTexture(tiendaTex);
}

// ---------- logica ----------
static bool tienda_abierta()
{
    return tiendaAbierta;
}

static bool tienda_tiene(int hab)
{
    return habilidades[hab].comprada;
}

static bool tienda_cerca(Player *j, int pantalla)
{
    return pantalla == PANTALLA_TIENDA &&
           CheckCollisionRecs(j->rec, tiendaIcono);
}

static void tienda_actualizar(Player *j, int pantalla)
{
    if (!tiendaAbierta)
    {
        if (tienda_cerca(j, pantalla) && IsKeyPressed(KEY_E))
        {
            tiendaAbierta = true;
            tiendaSel = 0;
        }
        return;
    }

    if (IsKeyPressed(KEY_DOWN))
        tiendaSel = (tiendaSel + 1) % NUM_HAB;
    if (IsKeyPressed(KEY_UP))
        tiendaSel = (tiendaSel + NUM_HAB - 1) % NUM_HAB;

    if (IsKeyPressed(KEY_ENTER))
    {
        Habilidad *h = &habilidades[tiendaSel];
        if (!h->comprada && j->monedas >= h->precio)
        {
            j->monedas -= h->precio;
            h->comprada = true;
        }
    }

    if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_BACKSPACE))
        tiendaAbierta = false;
}

// ---------- dibujo ----------
static void tienda_dibujar_icono(Player *j, int pantalla)
{
    if (pantalla != PANTALLA_TIENDA)
        return;

    if (tiendaTex.id != 0)
        DrawTexturePro(tiendaTex, {0, 0, (float)tiendaTex.width, (float)tiendaTex.height},
                       tiendaIcono, Vector2{0, 0}, 0.0f, WHITE);
    else
        DrawRectangleRec(tiendaIcono, GOLD); // marcador temporal

    if (tienda_cerca(j, pantalla) && !tiendaAbierta)
        DrawText("E: Entrar", (int)tiendaIcono.x, (int)tiendaIcono.y - 30, 24, WHITE);
}

static void tienda_dibujar_menu(Player *j)
{
    if (!tiendaAbierta)
        return;

    int sw = GetScreenWidth(), sh = GetScreenHeight();
    DrawRectangle(0, 0, sw, sh, Fade(BLACK, 0.6F));

    Rectangle panel = {sw / 2.0F - 350, sh / 2.0F - 320, 700, 640};
    DrawRectangleRec(panel, Fade(DARKGRAY, 0.95F));
    DrawRectangleLinesEx(panel, 4, GOLD);

    DrawText("TIENDA", (int)panel.x + 30, (int)panel.y + 20, 40, GOLD);
    DrawText(TextFormat("Monedas: %d", j->monedas),
             (int)panel.x + 450, (int)panel.y + 30, 28, YELLOW);

    for (int i = 0; i < NUM_HAB; i++)
    {
        Habilidad *h = &habilidades[i];
        int y = (int)panel.y + 100 + i * 90;

        Rectangle fila = {panel.x + 20, (float)y, panel.width - 40, 78};

        Color colorFila;
        if (i == tiendaSel)
            colorFila = Fade(GOLD, 0.35F);
        else
            colorFila = Fade(BLACK, 0.4F);
        DrawRectangleRec(fila, colorFila);

        if (i == tiendaSel)
            DrawRectangleLinesEx(fila, 2, GOLD);

        DrawText(h->nombre, (int)fila.x + 15, y + 10, 30, WHITE);
        DrawText(h->descripcion, (int)fila.x + 15, y + 46, 20, LIGHTGRAY);

        if (h->comprada)
        {
            DrawText("COMPRADO", (int)fila.x + 480, y + 25, 26, GREEN);
        }
        else
        {
            Color colorPrecio;
            if (j->monedas >= h->precio)
                colorPrecio = YELLOW;
            else
                colorPrecio = RED;
            DrawText(TextFormat("%d $", h->precio), (int)fila.x + 520, y + 25, 28, colorPrecio);
        }
    }

    DrawText("ARRIBA/ABAJO: elegir   ENTER: comprar   E: salir",
             (int)panel.x + 30, (int)panel.y + 600, 20, LIGHTGRAY);
}