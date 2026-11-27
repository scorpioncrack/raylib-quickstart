#pragma once
#include "eventos.h"

enum
{
    ESTADO_MENU,
    ESTADO_JUEGO,
    ESTADO_OPCIONES
};
int estado = ESTADO_MENU;
bool salir = false;
bool partidaIniciada = false;

enum
{
    OPC_JUGAR,
    OPC_OPCIONES,
    OPC_SALIR,
    NUM_OPC
};
int menuSel = 0;

void texto_centrado(const char *texto, int y, int tam, Color color)
{
    int ancho = MeasureText(texto, tam);
    DrawText(texto, (GetScreenWidth() - ancho) / 2, y, tam, color);
}

void menu_actualizar()
{
    if (IsKeyPressed(KEY_DOWN) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN))
        menuSel = (menuSel + 1) % NUM_OPC;

    if (IsKeyPressed(KEY_UP) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP))
        menuSel = (menuSel + NUM_OPC - 1) % NUM_OPC;

    if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
    {
        if (menuSel == OPC_JUGAR)
        {
            estado = ESTADO_JUEGO;
            partidaIniciada = true;
        }
        if (menuSel == OPC_OPCIONES)
        {
            estado = ESTADO_OPCIONES;
        }
        if (menuSel == OPC_SALIR)
        {
            salir = true;
        }
    }
}

void menu_dibujar(Texture2D fondo)
{
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    fondo1(fondo);
    DrawRectangle(0, 0, sw, sh, Fade(BLACK, 0.5F));

    texto_centrado("THE ABYSS", sh / 4, 120, GOLD);

    const char *textos[NUM_OPC];
    if (partidaIniciada)
        textos[OPC_JUGAR] = "Continuar";
    else
        textos[OPC_JUGAR] = "Jugar";
    textos[OPC_OPCIONES] = "Opciones";
    textos[OPC_SALIR] = "Salir";

    for (int i = 0; i < NUM_OPC; i++)
    {
        int y = sh / 2 + i * 90;
        if (i == menuSel)
        {
            texto_centrado(TextFormat("> %s <", textos[i]), y, 50, GOLD);
        }
        else
        {
            texto_centrado(textos[i], y, 50, WHITE);
        }
    }

    texto_centrado("ARRIBA/ABAJO: elegir     ENTER: aceptar", sh - 80, 24, LIGHTGRAY);
}

void opciones_actualizar()
{
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_ENTER) ||
        IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) ||
        IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
    {
        estado = ESTADO_MENU;
    }
}

void opciones_dibujar(Texture2D fondo)
{
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    fondo1(fondo);
    DrawRectangle(0, 0, sw, sh, Fade(BLACK, 0.7F));

    texto_centrado("OPCIONES", 100, 80, GOLD);
    texto_centrado("Teclado / Mando", 210, 36, LIGHTGRAY);

    const char *lineas[] = {
        "Mover: Flechas / Stick izquierdo",
        "Saltar: Z / A",
        "Espada: X / X",
        "Dash: C / B",
        "Lanza de sangre: F / Y",
        "Murcielago: A / Start",
        "Curacion: S / Cruceta abajo",
        "Fortalecimiento: D / Cruceta arriba",
        "Tienda: E",
        "Pausa: ESC"};
    int total = sizeof(lineas) / sizeof(lineas[0]);

    for (int i = 0; i < total; i++)
        texto_centrado(lineas[i], 290 + i * 55, 36, WHITE);

    texto_centrado("ESC / ENTER: volver", sh - 80, 24, LIGHTGRAY);
}