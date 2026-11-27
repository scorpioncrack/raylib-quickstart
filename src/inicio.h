// PANTALLA DE INICIO (libreria autocontenida)
#pragma once
#include "raylib.h"
#include <cmath>

#define NUM_OPC_INICIO 3
#define NUM_BRASAS 70

static const char *opcionesInicio[NUM_OPC_INICIO] = {"JUGAR", "CONTROLES", "SALIR"};
static int inicioSel = 0;
static bool inicioEnPantalla = true;
static bool inicioSalirJuego = false;
static bool inicioVerControles = false;
static Texture2D inicioTex = {0};    // fondo del juego
static Texture2D inicioHeroe = {0};  // sprite del personaje
static float inicioTiempo = 0.0F;
static float inicioBarra = 0.0F; // animacion del indicador de seleccion

typedef struct Brasa
{
    float x, y, vel, tam, fase;
} _brasa;

static Brasa brasas[NUM_BRASAS];

static void inicio_brasa_reset(Brasa *b, bool inicial)
{
    b->x = (float)GetRandomValue(0, GetScreenWidth());
    b->y = inicial ? (float)GetRandomValue(0, GetScreenHeight())
                   : (float)GetScreenHeight() + 10.0F;
    b->vel = 20.0F + (float)GetRandomValue(0, 50);
    b->tam = 1.5F + (float)GetRandomValue(0, 25) / 10.0F;
    b->fase = (float)GetRandomValue(0, 628) / 100.0F;
}

// ---------- carga / descarga ----------
static void inicio_cargar()
{
    inicioTex = LoadTexture("fondo1.png");                                 // fondo real del juego
    inicioHeroe = LoadTexture("protagonista/personaje_principal.png");     // tu vampiro
    if (inicioHeroe.id != 0)
        SetTextureFilter(inicioHeroe, TEXTURE_FILTER_POINT);
    for (int i = 0; i < NUM_BRASAS; i++)
        inicio_brasa_reset(&brasas[i], true);
}

static void inicio_descargar()
{
    if (inicioTex.id != 0)
        UnloadTexture(inicioTex);
    if (inicioHeroe.id != 0)
        UnloadTexture(inicioHeroe);
}

// ---------- estado ----------
static bool inicio_activo()
{
    return inicioEnPantalla;
}

static bool inicio_salir()
{
    return inicioSalirJuego;
}

// ---------- logica ----------
static void inicio_actualizar()
{
    float dt = GetFrameTime();
    inicioTiempo += dt;

    // brasas flotando hacia arriba
    for (int i = 0; i < NUM_BRASAS; i++)
    {
        Brasa *b = &brasas[i];
        b->y -= b->vel * dt;
        b->x += sinf(inicioTiempo * 1.2F + b->fase) * 12.0F * dt;
        if (b->y < -10.0F)
            inicio_brasa_reset(b, false);
    }

    // el indicador se desliza suavemente hacia la opcion elegida
    inicioBarra += ((float)inicioSel - inicioBarra) * 12.0F * dt;

    if (inicioVerControles)
    {
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_ESCAPE))
            inicioVerControles = false;
        return;
    }

    if (IsKeyPressed(KEY_DOWN))
        inicioSel = (inicioSel + 1) % NUM_OPC_INICIO;
    if (IsKeyPressed(KEY_UP))
        inicioSel = (inicioSel + NUM_OPC_INICIO - 1) % NUM_OPC_INICIO;

    if (IsKeyPressed(KEY_ENTER))
    {
        if (inicioSel == 0)
            inicioEnPantalla = false;
        else if (inicioSel == 1)
            inicioVerControles = true;
        else
            inicioSalirJuego = true;
    }
}

// ---------- dibujo ----------
static void inicio_dibujar_controles(int sw, int sh)
{
    DrawRectangle(0, 0, sw, sh, Fade(BLACK, 0.7F));

    Rectangle panel = {sw * 0.08F, sh * 0.2F, 760, 600};
    DrawRectangleRec(panel, Fade(Color{20, 4, 10, 255}, 0.95F));
    DrawRectangleLinesEx(panel, 2, Color{200, 30, 50, 255});

    DrawText("CONTROLES", (int)panel.x + 40, (int)panel.y + 30, 50, Color{230, 50, 70, 255});
    DrawRectangle((int)panel.x + 40, (int)panel.y + 90, 120, 3, Color{200, 30, 50, 255});

    const char *teclas[] = {"FLECHAS", "Z", "X", "C", "A", "F", "E"};
    const char *accion[] = {"Moverte (arriba/abajo al volar)", "Saltar", "Atacar con la espada",
                            "Dash", "Transformarte en murcielago", "Lanza de sangre", "Entrar a la tienda"};
    for (int i = 0; i < 7; i++)
    {
        int y = (int)panel.y + 120 + i * 62;
        DrawText(teclas[i], (int)panel.x + 40, y, 30, GOLD);
        DrawText(accion[i], (int)panel.x + 230, y + 3, 26, LIGHTGRAY);
    }

    DrawText("ENTER: volver", (int)panel.x + 40, (int)panel.y + 560, 22, GRAY);
}

static void inicio_dibujar()
{
    int sw = GetScreenWidth(), sh = GetScreenHeight();

    ClearBackground(BLACK);
    if (inicioTex.id != 0)
        DrawTexturePro(inicioTex, {0, 0, (float)inicioTex.width, (float)inicioTex.height},
                       {0, 0, (float)sw, (float)sh}, Vector2{0, 0}, 0.0f, WHITE);
    else
        DrawRectangle(0, 0, sw, sh, Color{10, 14, 30, 255});

    // oscurece el fondo y deja mas sombra a la izquierda para leer el menu
    DrawRectangle(0, 0, sw, sh, Fade(BLACK, 0.35F));
    DrawRectangleGradientH(0, 0, (int)(sw * 0.65F), sh, Fade(BLACK, 0.85F), Fade(BLACK, 0.0F));

    // personaje a la derecha (fila 0 = quieto), con resplandor rojo
    if (inicioHeroe.id != 0)
    {
        float anc = inicioHeroe.width / 4.0F; // FRAM columnas
        float alt = inicioHeroe.height / 3.0F; // FILAS filas
        int cuadro = (int)(inicioTiempo / 0.25F) % 4;
        float esc = 6.0F;
        float dw = anc * esc, dh = alt * esc;
        float px = sw * 0.72F - dw / 2, py = sh * 0.80F - dh; // pies sobre el "suelo"

        DrawCircleGradient(Vector2{px + dw / 2, py + dh * 0.55F}, dh * 0.7F,
                           Fade(Color{200, 20, 40, 255}, 0.30F), Fade(Color{200, 20, 40, 255}, 0.0F));
        DrawTexturePro(inicioHeroe, {cuadro * anc, 0, anc, alt}, {px, py, dw, dh},
                       Vector2{0, 0}, 0.0f, WHITE);
        DrawRectangle((int)(px - 60), (int)(sh * 0.80F), (int)(dw + 120), 6, Fade(Color{40, 60, 100, 255}, 0.8F));
    }

    // brasas
    for (int i = 0; i < NUM_BRASAS; i++)
    {
        Brasa *b = &brasas[i];
        float parpadeo = 0.5F + 0.5F * sinf(inicioTiempo * 3.0F + b->fase);
        DrawCircleV({b->x, b->y}, b->tam, Fade(Color{255, 90, 60, 255}, 0.25F + 0.55F * parpadeo));
    }

    // ---- titulo (izquierda) ----
    int x = (int)(sw * 0.08F);
    int yTit = (int)(sh * 0.17F);
    float pulso = 0.85F + 0.15F * sinf(inicioTiempo * 2.0F);

    DrawText("THE", x + 6, yTit - 70, 56, Fade(Color{230, 230, 235, 255}, 0.85F));
    // sombra / resplandor del titulo
    DrawText("ABYSS", x + 4, yTit + 4, 150, Fade(Color{120, 10, 25, 255}, 0.9F));
    DrawText("ABYSS", x, yTit, 150, Fade(Color{235, 40, 60, 255}, pulso));
    DrawRectangle(x + 6, yTit + 165, 360, 3, Color{200, 30, 50, 255});
    DrawText("Desciende. Sobrevive.", x + 6, yTit + 185, 28, Fade(LIGHTGRAY, 0.8F));

    // ---- opciones ----
    int yOpc = (int)(sh * 0.58F);
    int paso = 80;

    // barra roja que se desliza
    float by = yOpc + inicioBarra * paso;
    DrawRectangle(x - 24, (int)by + 4, 6, 50, Color{235, 40, 60, 255});
    DrawRectangleGradientH(x - 18, (int)by + 4, 380, 50,
                           Fade(Color{235, 40, 60, 255}, 0.28F), Fade(Color{235, 40, 60, 255}, 0.0F));

    for (int i = 0; i < NUM_OPC_INICIO; i++)
    {
        bool sel = (i == inicioSel);
        int desp = sel ? 14 : 0;
        DrawText(opcionesInicio[i], x + desp, yOpc + i * paso, 46,
                 sel ? RAYWHITE : Fade(LIGHTGRAY, 0.55F));
    }

    DrawText("ARRIBA/ABAJO  elegir      ENTER  aceptar", x, sh - 70, 22, Fade(GRAY, 0.9F));

    if (inicioVerControles)
        inicio_dibujar_controles(sw, sh);
}

// ---------- UNA SOLA FUNCION PARA TODA LA LIBRERIA ----------
// Devuelve true mientras el menu esta en pantalla.
static bool inicio_correr()
{
    static bool cargado = false;
    if (!cargado)
    {
        inicio_cargar();       // se carga sola la primera vez
        SetExitKey(KEY_NULL);  // ESC ya no cierra la ventana (se sale con SALIR)
        cargado = true;
    }

    if (!inicio_activo())
        return false;

    inicio_actualizar();
    BeginDrawing();
    inicio_dibujar();
    EndDrawing();
    return true;
}