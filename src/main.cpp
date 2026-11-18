#include "eventos.h"

// fondo del juego
void fondo1();
int main()
{
    InitWindow(1100, 800, "THE ABYSS");
    ToggleBorderlessWindowed(); // requiere raylib 5.0+; si no existe usa ToggleFullscreen()
    SearchAndSetResourceDir("resources");

    Texture2D jugador_text = LoadTexture("personaje.png");
    Texture2D fondo_text = LoadTexture("fondo1.png");

    if (jugador_text.id == 0)
    {
        TraceLog(LOG_WARNING, "NO CARGO personaje.png");
    }
    SetTextureFilter(jugador_text, TEXTURE_FILTER_POINT);
    int anc = jugador_text.width / FRAM;
    int alt = jugador_text.height;
    SetTargetFPS(60);

    switch (jugador1.tipo)
    {
    case 1:
        jugador1.arma = espada;
        break;
    }

    while (!WindowShouldClose())
    {
        act_jug(&jugador1); // timers, animacion, teclas, ataque y movimiento
        patrulla();

        BeginDrawing();
        ClearBackground(GRAY);
        // dibujar objetos
        DrawText(TextFormat("vidas %d", jugador1.vida), 200, 200, 40, WHITE);
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

    UnloadTexture(fondo_text);
    UnloadTexture(jugador_text);
    UnloadTexture(fondo_text); // <-- agregar

    CloseWindow();
    return 0;
}

void fondo1(Texture2D fondo_text)
{

    DrawTexturePro(fondo_text, origen, destino, Vector2{0, 0}, 0.0f, WHITE);
}
