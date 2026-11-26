// JESUS GMALIEL AVILEZ SUAREZ
#pragma once
#include "propiedades.h"
#include "mas.h" // libreria de la tienda (HAB_VOLAR, HAB_DASH, HAB_SANGRE, tienda_tiene)
#include <cmath> // fabsf
#define NUM_FONDOS 3
#define ANCHO_PANTALLA 1920
#define LARGO_pantalla 1080

Texture2D fondos[NUM_FONDOS];
int fondo_inicial = 0;

Objeto areas[2];

// variables del jugador que usan act_jug y el dibujado del main
float timer = 100.0F, espadaVisible = 0.0F, dashEspera = 0.0F, dashDuracion = 0.0F, lanzaDuracion = 0.0F;
int lanzaDir = 1; // direccion en la que va la lanza (se fija al lanzarla)
float tiem = 0.0F;
int fra = 0;
int fila_anim = 0; // 0 quieto, 1 correr, 2 saltar

// actualizar
void act_jug(Player *entidad);
// movimiento y colisiones
void movimiento(Player *entidad);
void objetos(Player *entidad);
void plataforma(Player *entidad, Objeto *plataforma);
void dash(Player *entidad);
// habilidades
void lanza_sangre();
void curar(Player &entidad);
// combate
void ataque(Player *jugador);
void impacto(Player *jugador, Enemigo *enemigo);
void enemigo_ataque(Player &jugador);
// enemigos
void enemigos(Player jugador);
void patrulla();

// ==================== ACTUALIZAR ====================

void act_jug(Player *entidad)
{
    // timers
    timer += GetFrameTime();
    espadaVisible -= GetFrameTime();
    entidad->invul -= GetFrameTime();
    dashEspera -= GetFrameTime();
    dashDuracion -= GetFrameTime();
    lanzaDuracion -= GetFrameTime();

    // entradas del control (la animacion la maneja actualizar_animacion)
    float ejeX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
    float ejeY = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);

    // movimiento del jugador
    entidad->velX = 0;
    if (entidad->volar)
    {
        entidad->velY = 0;
        if (ejeY > 0.3F || IsKeyDown(KEY_DOWN))
            entidad->velY = SPEED;

        if (ejeY < -0.3F || IsKeyDown(KEY_UP))
            entidad->velY = -SPEED;
    }
    else
    {
        if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) || IsKeyPressed(KEY_Z))
        {
            if (entidad->saltar == 1)
            {
                entidad->saltar = 0;
                entidad->velY = -SALTO;
            }
        }
    }

    // activar / desactivar vuelo (bloqueado hasta comprarlo en la tienda)
    if ((IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT) || IsKeyPressed(KEY_A)) && tienda_tiene(HAB_VOLAR))
    {
        entidad->volar = !entidad->volar;
        if (entidad->volar)
        {
            // caja baja de 64 a 40, los pies quedan en el mismo lugar
            entidad->pos.y += 24;
            entidad->rec.height = 40;
        }
        else
        {
            entidad->pos.y -= 24;
            entidad->rec.height = 64;
        }
        entidad->rec.y = entidad->pos.y;
    }

    if (ejeX < -0.3F || IsKeyDown(KEY_LEFT))
        entidad->velX = -SPEED;
    if (ejeX > 0.3F || IsKeyDown(KEY_RIGHT))
        entidad->velX = SPEED;

    if (entidad->velX != 0)
        entidad->dir = (entidad->velX > 0) ? 1 : -1;

    // habilidades: dash (bloqueado hasta comprarlo)
    if ((IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) || IsKeyPressed(KEY_C)) && tienda_tiene(HAB_DASH))
    {
        if (dashEspera < 0)
        {
            dashEspera = DASH_ESPERA;
            dashDuracion = DASH_DURACION;
        }
    }
    if (dashDuracion > 0)
    {
        dash(entidad);
    }

    // habilidades: lanza de sangre (bloqueada hasta comprarla)
    if ((IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_UP) || IsKeyPressed(KEY_F)) && tienda_tiene(HAB_SANGRE))
    {
        if (entidad->sangre >= 1 && lanzaDuracion <= 0)
        {
            entidad->sangre -= 1.0F;
            lanzaDuracion = LANZA_DURACION;
            lanzaDir = entidad->dir;

            if (lanzaDir == 1)
                lanza.rango.x = entidad->pos.x + entidad->rec.width;
            else
                lanza.rango.x = entidad->pos.x - lanza.rango.width;
            lanza.rango.y = entidad->pos.y + 30.0F;
        }
    }
    if (lanzaDuracion > 0)
    {
        lanza_sangre();
    }

    // ataque del jugador
    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT) || IsKeyPressed(KEY_X))
    {
        if (timer >= entidad->arma.tiempo)
        {
            ataque(entidad);
            timer = 0.0F;
            espadaVisible = 0.15F;
        }
    }

    // dano de enemigos y movimiento
    enemigo_ataque(*entidad);
    movimiento(entidad);
}

// ==================== MOVIMIENTO Y COLISIONES ====================
void movimiento(Player *entidad)
{
    // movimiento en x
    entidad->pos.x += entidad->velX;
    entidad->rec.x = entidad->pos.x;
    entidad->eje = 1;
    objetos(entidad);

    // movimiento en y
    if (!entidad->volar)
    {
        entidad->velY += GRAVEDAD;
    }

    entidad->pos.y += entidad->velY;
    entidad->rec.y = entidad->pos.y;
    entidad->eje = 0;
    entidad->saltar = 0;
    objetos(entidad);
}

// objetos con colisiones
void objetos(Player *entidad)
{
    for (int i = 0; i < numObjetos; i++)
        plataforma(entidad, &escenario[i]);
}

void plataforma(Player *entidad, Objeto *plataforma)
{
    float dir;
    int pasos;
    bool seguir;

    if (CheckCollisionRecs(entidad->rec, plataforma->rect))
    {
        if (plataforma->blocking)
        {
            if (entidad->eje == 0)
            {
                entidad->pos.y -= entidad->velY;
                dir = (entidad->velY > 0) ? 1 : -1;
                pasos = 0;
                entidad->rec.y = entidad->pos.y + dir;
                seguir = true;
                while (seguir)
                {
                    if (!CheckCollisionRecs(entidad->rec, plataforma->rect))
                    {
                        if (pasos < fabsf(entidad->velY))
                        {
                            entidad->pos.y += dir;
                            entidad->rec.y = entidad->pos.y + dir;
                            pasos++;
                        }
                        else
                            seguir = false;
                    }
                    else
                        seguir = false;
                }
                entidad->rec.y = entidad->pos.y;

                if (entidad->velY > 0)
                    entidad->saltar = 1;
                entidad->velY = 0;
            }

            if (entidad->eje == 1)
            {
                entidad->pos.x -= entidad->velX;
                dir = (entidad->velX > 0) ? 1 : -1;
                pasos = 0;
                entidad->rec.x = entidad->pos.x + dir;
                seguir = true;
                while (seguir)
                {
                    if (!CheckCollisionRecs(entidad->rec, plataforma->rect))
                    {
                        if (pasos < fabsf(entidad->velX))
                        {
                            entidad->pos.x += dir;
                            entidad->rec.x = entidad->pos.x + dir;
                            pasos++;
                        }
                        else
                            seguir = false;
                    }
                    else
                        seguir = false;
                }
                entidad->rec.x = entidad->pos.x;
            }
        }
    }
}

// ==================== HABILIDADES ====================

void dash(Player *entidad)
{
    if (entidad->dir == 1)
    {
        entidad->velX = DASH;
    }
    else
    {
        entidad->velX = -DASH;
    }
}

void lanza_sangre()
{
    // avanzar en la direccion fijada al lanzar
    lanza.rango.x += lanzaDir * VEL_LANZA;

    // chocar con paredes/plataformas
    for (int i = 0; i < numObjetos; i++)
    {
        if (escenario[i].blocking && CheckCollisionRecs(lanza.rango, escenario[i].rect))
        {
            lanzaDuracion = 0;
            return;
        }
    }

    // chocar con enemigos
    for (int i = 0; i < numEnemigos; i++)
    {
        Enemigo *enemigo = &lista_enemigos[i];
        if (enemigo->estado)
        {
            if (CheckCollisionRecs(lanza.rango, enemigo->base.rec))
            {
                enemigo->base.vida -= lanza.dano;
                if (enemigo->base.vida <= 0)
                {
                    enemigo->estado = 0;
                    jugador1.monedas += 5; // monedas tambien al matar con la lanza
                }
                lanzaDuracion = 0; // la lanza desaparece al golpear
                return;
            }
        }
    }
}

// curar (por referencia para que de verdad modifique al jugador)
void curar(Player &entidad)
{
    entidad.vida += 1;
}

// ==================== COMBATE ====================

void ataque(Player *atacante)
{
    if (atacante->dir == 1)
        atacante->arma.rango.x = atacante->pos.x + atacante->rec.width;
    else
        atacante->arma.rango.x = atacante->pos.x - atacante->arma.rango.width;
    atacante->arma.rango.y = atacante->pos.y + 30.0F;
    enemigos(*atacante);
}

void impacto(Player *jugador, Enemigo *enemigo)
{
    if (CheckCollisionRecs(jugador->arma.rango, enemigo->base.rec))
    {
        if (enemigo->estado)
        {
            int danio = 0;
            switch (jugador->arma.tipo)
            {
            case 1:
                danio = jugador->arma.dano;
                break;
            }

            // cada golpe de espada recarga sangre
            if (jugador1.sangre + 0.3 <= 1)
            {
                jugador1.sangre += .3;
            }
            else
            {
                jugador1.sangre = 1.0;
            }

            enemigo->base.vida -= danio;
            if (enemigo->base.vida <= 0)
            {
                enemigo->estado = 0;
                jugador1.monedas += 5; // monedas al matar un enemigo
            }
        }
    }
}

void enemigo_ataque(Player &jugador)
{
    bool golpeado = false;

    if (jugador.invul > 0)
        return;
    if (jugador.vida <= 0)
        return;

    for (int i = 0; i < numEnemigos; i++)
    {
        if (!golpeado)
        {
            Enemigo *atacante = &lista_enemigos[i];
            if (atacante->estado)
            {
                if (atacante->base.dir == 1)
                    atacante->ataque.rango.x = atacante->base.pos.x + atacante->base.rec.width;
                else
                    atacante->ataque.rango.x = atacante->base.pos.x - atacante->ataque.rango.width;
                atacante->ataque.rango.y = atacante->base.pos.y;

                if (CheckCollisionRecs(atacante->base.rec, jugador.rec))
                {
                    jugador.vida -= atacante->ataque.dano;
                    jugador.invul = IFRAME;
                    if (jugador.vida <= 0)
                        jugador.estado = 0;
                    golpeado = true;
                }
            }
        }
    }
}

// ==================== ENEMIGOS ====================

void enemigos(Player jugador)
{
    for (int i = 0; i < numEnemigos; i++)
        impacto(&jugador, &lista_enemigos[i]);
}

void patrulla()
{
    for (int i = 0; i < numEnemigos; i++)
    {
        Enemigo *enemigo = &lista_enemigos[i];

        if (enemigo->estado)
        {
            if (enemigo->desplazado >= enemigo->distancia)
            {
                enemigo->base.dir *= -1;
                enemigo->desplazado = 0;
            }

            enemigo->base.velX = enemigo->base.dir * enemigo->velocidad;
            movimiento(&enemigo->base);
            enemigo->desplazado += fabsf(enemigo->base.velX);
        }
    }
}