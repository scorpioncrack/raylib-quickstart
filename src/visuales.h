// MELANY DENISSE FLORES ALVAREZ 
void textura_plataformas(Texture2D tex, Rectangle rect)
{
    Rectangle origen = {0, 0, (float)tex.width, (float)tex.height};
    DrawTexturePro(tex, origen, rect, Vector2{0, 0}, 0.0f, WHITE);
}

void fondo1(Texture2D fondo_text)
{
    Rectangle origen = {0, 0, (float)fondo_text.width, (float)fondo_text.height};
    Rectangle destino = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};

    DrawTexturePro(fondo_text, origen, destino, Vector2{0, 0}, 0.0f, WHITE);
}
