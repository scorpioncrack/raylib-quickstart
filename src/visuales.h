// MELANY DENISSE FLORES ALVAREZ
#pragma once
#include "eventos.h"
#define FILAS 3
#define ESCALA_SPRITE 0.8f
enum
{
    FILA_QUIETO = 0,
    FILA_CORRER = 1,
    FILA_SALTO = 2
};

// murcielago (1 fila, 4 cuadros)
#define FMUR 4
#define MURW 90.0f // ancho al dibujar, sube o baja para ajustar
float tmu = 0.0f;
int fmu = 0;

// texturas de corazones
Texture2D corazon_rojo;
Texture2D corazon_oscuro;

void corazones_vida(Player *vida)
{
    int maxVidas = 5;
    float margen = 20.0f;     // Distancia desde la esquina superior izquierda
    float tamano = 100.0f;    // Ancho y alto de cada corazon
    float separacion = 45.0f; // Espacio entre cada corazon

    for (int i = 0; i < maxVidas; i++)
    {
        Texture2D texActual;
        if (i < vida->vida)
            texActual = corazon_rojo;
        else
            texActual = corazon_oscuro;

        Rectangle origen = {0, 0, (float)texActual.width, (float)texActual.height};
        Rectangle destino = {margen + (i * separacion), margen, tamano, tamano};

        DrawTexturePro(texActual, origen, destino, Vector2{0, 0}, 0.0f, WHITE);
    }
}

void textura_plataformas(Texture2D tex, Rectangle rect)
{
    Rectangle origen = {0, 0, (float)tex.width, (float)tex.height};
    DrawTexturePro(tex, origen, rect, Vector2{0, 0}, 0.0f, WHITE);
}

void cargar(int pan)
{
    if (pan == 0)
    {
        escenario = esc0;
        numObjetos = sizeof(esc0) / sizeof(esc0[0]);
        lista_enemigos = ene0;
        numEnemigos = sizeof(ene0) / sizeof(ene0[0]);
    }
    if (pan == 1)
    {
        escenario = esc1;
        numObjetos = sizeof(esc1) / sizeof(esc1[0]);
        lista_enemigos = ene1;
        numEnemigos = sizeof(ene1) / sizeof(ene1[0]);
    }
    if (pan == 2)
    {
        escenario = esc2;
        numObjetos = sizeof(esc2) / sizeof(esc2[0]);
        lista_enemigos = ene2;
        numEnemigos = sizeof(ene2) / sizeof(ene2[0]);
    }

    // la lanza en vuelo desaparece al cambiar de pantalla
    lanzaDuracion = 0;
}

void pantallas(Player *jugador)
{
    // salir por la derecha
    if (CheckCollisionRecs(areas[1].rect, jugador->rec))
    {
        if (fondo_inicial < NUM_FONDOS - 1)
        {
            jugador->pos.x = 40;
            jugador->rec.x = 40;
            fondo_inicial++;
            cargar(fondo_inicial);
        }
        else
        {
            // ultima pantalla: no dejar pasar
            jugador->pos.x = areas[1].rect.x - jugador->rec.width;
            jugador->rec.x = jugador->pos.x;
        }
    }

    // salir por la izquierda
    if (fondo_inicial == 0)
    {
        // primera pantalla: no dejar pasar
        if (jugador->rec.x < 0)
        {
            jugador->pos.x = 0;
            jugador->rec.x = 0;
        }
    }
    else
    {
        if (CheckCollisionRecs(areas[0].rect, jugador->rec))
        {
            jugador->pos.x = ANCHO_PANTALLA - jugador->rec.width - 40;
            jugador->rec.x = jugador->pos.x;
            fondo_inicial--;
            cargar(fondo_inicial);
        }
    }
}

void fondo1(Texture2D fondo_text)
{
    Rectangle origen = {0, 0, (float)fondo_text.width, (float)fondo_text.height};
    Rectangle destino = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};

    DrawTexturePro(fondo_text, origen, destino, Vector2{0, 0}, 0.0f, WHITE);
}

void actualizar_animacion(Player *j)
{
    int nueva;
    if (j->volar || j->saltar != 1)
        nueva = FILA_SALTO;
    else if (j->velX != 0)
        nueva = FILA_CORRER;
    else
        nueva = FILA_QUIETO;

    if (nueva != fila_anim)
    {
        fila_anim = nueva;
        fra = 0;
        tiem = 0;
    }

    if (fila_anim == FILA_SALTO)
    {
        if (j->volar)
            fra = 2;
        else if (j->velY < -5)
            fra = 0;
        else if (j->velY < -1.5f)
            fra = 1;
        else if (j->velY < 1.5f)
            fra = 2;
        else
            fra = 3;
    }
    else
    {
        float paso;
        if (fila_anim == FILA_CORRER)
            paso = 0.10f;
        else
            paso = 0.25f;

        tiem += GetFrameTime();
        if (tiem >= paso)
        {
            tiem -= paso;
            fra = (fra + 1) % FRAM;
        }
    }
}

// aleteo del murcielago
void anim_murc()
{
    tmu += GetFrameTime();
    if (tmu >= 0.08f)
    {
        tmu -= 0.08f;
        fmu = (fmu + 1) % FMUR;
    }
}

void dibujar_jugador(Texture2D tex, Player *j, Color tinte)
{
    float anc = tex.width / (float)FRAM;
    float alt = tex.height / (float)FILAS;

    float anchoOrigen = anc;
    if (j->dir < 0)
        anchoOrigen = -anc;

    Rectangle ori = {fra * anc, fila_anim * alt, anchoOrigen, alt};

    float w = anc * ESCALA_SPRITE;
    float h = alt * ESCALA_SPRITE;
    Rectangle dest = {j->rec.x + j->rec.width / 2 - w / 2,
                      j->rec.y + j->rec.height - h, w, h};

    DrawTexturePro(tex, ori, dest, Vector2{0, 0}, 0.0f, tinte);
}

// dibuja el murcielago centrado en la caja
void dibujar_murc(Texture2D tex, Player *j, Color tinte)
{
    float anc = tex.width / (float)FMUR;
    float alt = (float)tex.height;

    float anchoOrigen = anc;
    if (j->dir < 0)
        anchoOrigen = -anc;

    Rectangle ori = {fmu * anc, 0, anchoOrigen, alt};

    float w = MURW;
    float h = alt * w / anc;
    Rectangle dest = {j->rec.x + j->rec.width / 2 - w / 2,
                      j->rec.y + j->rec.height / 2 - h / 2, w, h};

    DrawTexturePro(tex, ori, dest, Vector2{0, 0}, 0.0f, tinte);
}