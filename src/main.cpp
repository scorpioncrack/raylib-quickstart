#include "propiedades.h"
#include <cmath>
#define FRAM 4
// movimiento y colisiones
void movimiento(Player *entidad);
void objetos(Player *entidad);
void plataforma(Player *entidad, Objeto *plataforma);
// combate
void ataque(Player *jugador);
void impacto(Player jugador, Enemigo *enemigo);
void enemigo_ataque(Player &jugador);
// enemigos
void enemigos(Player jugador);
void patrulla();

int main()
{
    float timer = 100.0F, espadaVisible = 0.0F; 
    InitWindow(1100, 800, "THE ABYSS");
    ToggleBorderlessWindowed();
    SearchAndSetResourceDir("resources");
    Texture2D jugador_text = LoadTexture("personaje.png");
    if (jugador_text.id == 0)
    {
        TraceLog(LOG_WARNING, "NO CARGO personaje.png");
    }
    SetTextureFilter(jugador_text, TEXTURE_FILTER_POINT);
    int anc = jugador_text.width / FRAM;
    int alt = jugador_text.height;
    int fra = 0;
    float tiem = 0.0F;
    SetTargetFPS(60);

    switch (jugador1.tipo)
    {
    case 1:
        jugador1.arma = espada;
        break;
    }

    while (!WindowShouldClose())
    {
        timer += GetFrameTime();
        espadaVisible -= GetFrameTime();
        jugador1.invul -= GetFrameTime();

        
        tiem += GetFrameTime();
        if (tiem >= 0.18F)
        {
            tiem = 0.0F;
            fra = (fra + 1) % FRAM;
        }

        // movimiento de jugador
        jugador1.velX = 0;
        if (IsKeyDown(KEY_LEFT))
            jugador1.velX = -SPEED;
        if (IsKeyDown(KEY_RIGHT))
            jugador1.velX = SPEED;
        if (IsKeyDown(KEY_UP))
        {
            if (jugador1.saltar == 1)
            {
                jugador1.saltar = 0;
                jugador1.velY = -SALTO;
            }
        }
        if (jugador1.velX != 0)
            jugador1.dir = (jugador1.velX > 0) ? 1 : -1;

        // ataque del jugador
        if (IsKeyDown(KEY_X))
        {
            if (timer >= jugador1.arma.tiempo)
            {
                ataque(&jugador1);
                timer = 0.0F;
                espadaVisible = 0.15F; 
            }
        }
//ola mundo
// mensaje de prueba

        // ataque enemigos
        enemigo_ataque(jugador1);
        movimiento(&jugador1);
        patrulla();
        BeginDrawing();
        ClearBackground(GRAY);
        // dibujar objetos
        DrawText((TextFormat("vidas %d", jugador1.vida)), 200, 200, 40, WHITE);
        for (int i = 0; i < numObjetos; i++)
            DrawRectangleRec(escenario[i].rect, escenario[i].color);

        
        Rectangle ori = {(float)(fra * anc), 0, (float)anc, (float)alt};
        if (jugador1.dir < 0)
            ori.width = -(float)anc;

        Color tinte = WHITE; 
        if (jugador1.invul > 0)
        {
            if ((int)(jugador1.invul * 10) % 2 == 0)
                tinte = Fade(WHITE, 0.3F);
        }

        DrawTexturePro(
            jugador_text,
            ori,
            Rectangle{jugador1.pos.x, jugador1.pos.y, jugador1.rec.width, jugador1.rec.height},
            Vector2{0, 0},
            0.0f, tinte);
        for (int i = 0; i < numEnemigos; i++)
        {
            if (lista_enemigos[i].estado)
                DrawRectangleRec(lista_enemigos[i].base.rec, lista_enemigos[i].color);
        }

        if (espadaVisible > 0)
            DrawRectangleRec(jugador1.arma.rango, jugador1.arma.color);

        EndDrawing();
    }
    UnloadTexture(jugador_text);
    CloseWindow();
    return 0;
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
    entidad->velY += GRAVEDAD;
    entidad->pos.y += entidad->velY;
    entidad->rec.y = entidad->pos.y;
    entidad->eje = 0;
    entidad->saltar = 0;
    objetos(entidad);
}

// objetos con coliciones
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

// ==================== COMBATE ====================

void ataque(Player *atacante)
{
    if (atacante->dir == 1)
        atacante->arma.rango.x = atacante->pos.x + atacante->rec.width;
    else
        atacante->arma.rango.x = atacante->pos.x - atacante->arma.rango.width;
    atacante->arma.rango.y = atacante->pos.y;
    enemigos(*atacante);
}

void impacto(Player jugador, Enemigo *enemigo)
{
    if (CheckCollisionRecs(jugador.arma.rango, enemigo->base.rec))
    {
        if (enemigo->estado)
        {
            int daño = 0;
            switch (jugador.arma.tipo)
            {
            case 1:
                daño = jugador.arma.dano;
                break;
            }

            enemigo->base.vida -= daño;
            if (enemigo->base.vida <= 0)
                enemigo->estado = 0;
        }
    }
}

void enemigo_ataque(Player &jugador)
{
    bool golpeado = false;

    if (jugador.invul > 0)
    {
        if (jugador.vida <= 0)
            return;
    }

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
        impacto(jugador, &lista_enemigos[i]);
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