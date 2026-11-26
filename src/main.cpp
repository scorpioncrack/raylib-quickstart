#include "eventos.h"
#include "visuales.h"

int main()
{
    InitWindow(1920, 1080, "THE ABYSS");
    ToggleBorderlessWindowed();
    SearchAndSetResourceDir("resources");

    fondos[0] = LoadTexture("fondo1.png");
    fondos[1] = LoadTexture("fondo2.png");
    fondos[2] = LoadTexture("fondo1.png");

    Texture2D jugador_text = LoadTexture("protagonista/personaje_principal.png");
    Texture2D murc_text = LoadTexture("protagonista/personaje_volar.png");
    Texture2D plat_text = LoadTexture("plataforma.png");

    SetTextureFilter(jugador_text, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(murc_text, TEXTURE_FILTER_BILINEAR);

    areas[0].rect = {0, 0, 10, LARGO_pantalla};
    areas[1].rect = {ANCHO_PANTALLA - 10, 0, 10, LARGO_pantalla};

    SetTargetFPS(60);

    switch (jugador1.tipo)
    {
    case 1:
        jugador1.arma = espada;
        break;
    }

    while (!WindowShouldClose())
    {
        act_jug(&jugador1);
        pantallas(&jugador1);
        actualizar_animacion(&jugador1);
        anim_murc();
        patrulla();

        fondo_y_murc(plat_text, murc_text, jugador_text);
    }

    UnloadTexture(fondos[0]);
    UnloadTexture(fondos[1]);
    UnloadTexture(fondos[2]); // NUEVO
    UnloadTexture(jugador_text);
    UnloadTexture(murc_text); // NUEVO
    UnloadTexture(plat_text);
    CloseWindow();
    return 0;
}