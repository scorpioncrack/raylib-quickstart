#include "eventos.h"
#include "visuales.h"

int main()
{
    InitWindow(1100, 800, "THE ABYSS");
    ToggleBorderlessWindowed();
    SearchAndSetResourceDir("resources");

    Texture2D jugador_text = LoadTexture("protagonista/personaje_principal.png");
    Texture2D fondo_text = LoadTexture("fondo1.png");
    Texture2D plat_text = LoadTexture("plataforma.png");

    SetTextureFilter(jugador_text, TEXTURE_FILTER_BILINEAR);
    SetTargetFPS(60);

    switch (jugador1.tipo)
    {
    case 1:
        jugador1.arma = espada;
        break;
    }

    while (!WindowShouldClose())
    {
        // ---------- LÓGICA ----------
        
        act_jug(&jugador1);
        actualizar_animacion(&jugador1);
        patrulla();

        // dibujar dibujos
        BeginDrawing();
        ClearBackground(GRAY);
        fondo1(fondo_text);

        DrawText(TextFormat("vidas %d", jugador1.vida), 200, 200, 40, WHITE);
        for (int i = 0; i < numObjetos; i++)
            textura_plataformas(plat_text, escenario[i].rect);

        Color tinte = WHITE;
        if (jugador1.invul > 0)
        {
            if ((int)(jugador1.invul * 10) % 2 == 0)
                tinte = Fade(WHITE, 0.3F);
        }

        dibujar_jugador(jugador_text, &jugador1, tinte);

        for (int i = 0; i < numEnemigos; i++)
        {
            if (lista_enemigos[i].estado)
                DrawRectangleRec(lista_enemigos[i].base.rec, lista_enemigos[i].color);
        }

        if (armaVisible > 0)
            DrawRectangleRec(jugador1.arma.rango, jugador1.arma.color);

        if (lanzaDuracion > 0)
            DrawRectangleRec(lanza.rango, lanza.color);
        EndDrawing();
    }

    UnloadTexture(fondo_text);
    UnloadTexture(jugador_text);
    UnloadTexture(plat_text);
    CloseWindow();
    return 0;
}