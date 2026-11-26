#include "eventos.h"
#include "visuales.h"

int main()
{
    InitWindow(1920, 1080, "THE ABYSS");
    ToggleBorderlessWindowed();
    SearchAndSetResourceDir("resources");

    Texture2D jugador_text = LoadTexture("protagonista/personaje_principal.png");
    Texture2D murc_text = LoadTexture("protagonista/personaje_volar.png");
    fondos[0] = LoadTexture("fondo1.png");
    fondos[1] = LoadTexture("fondo2.png");
    fondos[2] = LoadTexture("fondo1.png"); // cambia por tu archivo de la pantalla 3

    // zonas de cambio de pantalla (bordes izquierdo y derecho)
    areas[0].rect = {0.0F, 0.0F, 10.0F, (float)LARGO_pantalla};
    areas[1].rect = {(float)(ANCHO_PANTALLA - 10), 0.0F, 10.0F, (float)LARGO_pantalla};

    Texture2D plat_text = LoadTexture("plataforma.png");

    SetTextureFilter(jugador_text, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(murc_text, TEXTURE_FILTER_BILINEAR);
    SetTargetFPS(60);

    switch (jugador1.tipo)
    {
    case 1:
        jugador1.arma = espada;
        break;
    }

    while (!WindowShouldClose())
    {
        // ---------- LOGICA ----------
        act_jug(&jugador1);
        pantallas(&jugador1);
        actualizar_animacion(&jugador1);
        anim_murc();
        

        // ---------- DIBUJO ----------
        BeginDrawing();
        ClearBackground(GRAY);

        fondo1(fondos[fondo_inicial]);

        DrawText(TextFormat("vidas %d", jugador1.vida), 200, 200, 40, WHITE);
        DrawText(TextFormat("sangre %.1f", jugador1.sangre), 200, 250, 40, WHITE);

        for (int i = 0; i < numObjetos; i++)
            textura_plataformas(plat_text, escenario[i].rect);

        Color tinte = WHITE;
        if (jugador1.invul > 0)
        {
            if ((int)(jugador1.invul * 10) % 2 == 0)
                tinte = Fade(WHITE, 0.3F);
        }

        // personaje o murcielago
        if (jugador1.volar)
        {
            dibujar_murc(murc_text, &jugador1, tinte);
        }
        else
        {
            dibujar_jugador(jugador_text, &jugador1, tinte);
        }

        for (int i = 0; i < numEnemigos; i++)
        {
            if (lista_enemigos[i].estado)
                DrawRectangleRec(lista_enemigos[i].base.rec, lista_enemigos[i].color);
        }

        if (espadaVisible > 0)
            DrawRectangleRec(jugador1.arma.rango, jugador1.arma.color);

        if (lanzaDuracion > 0)
            DrawRectangleRec(lanza.rango, lanza.color);

        EndDrawing();
    }

    UnloadTexture(fondos[0]);
    UnloadTexture(fondos[1]);
    UnloadTexture(fondos[2]);
    UnloadTexture(jugador_text);
    UnloadTexture(murc_text);
    UnloadTexture(plat_text);
    CloseWindow();
    return 0;
}