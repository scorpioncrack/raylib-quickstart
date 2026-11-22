// MELANY DENISSE FLORES ALVAREZ
#define FILAS 3
#define ESCALA_SPRITE 0.8f // el personaje mide ~67 px de alto; súbelo para hacerlo más grande
enum { FILA_QUIETO = 0, FILA_CORRER = 1, FILA_SALTO = 2 };

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
void actualizar_animacion(Player *j)
{
    int nueva;
    if (j->volar || j->saltar != 1)  nueva = FILA_SALTO;   // en el aire (saltar == 1 significa "en el suelo")
    else if (j->velX != 0)           nueva = FILA_CORRER;
    else                             nueva = FILA_QUIETO;

    if (nueva != fila_anim)          // al cambiar de animación se reinicia
    {
        fila_anim = nueva;
        fra = 0;
        tiem = 0;
    }

    if (fila_anim == FILA_SALTO)
    {
        // el cuadro depende de la velocidad vertical, no del tiempo
        if (j->volar)            fra = 2;
        else if (j->velY < -5)   fra = 0;   // despegue
        else if (j->velY < -1.5f) fra = 1;  // subiendo
        else if (j->velY < 1.5f) fra = 2;   // punto más alto
        else                     fra = 3;   // cayendo
    }
    else
    {
        float paso = (fila_anim == FILA_CORRER) ? 0.10f : 0.25f;  // segundos por cuadro
        tiem += GetFrameTime();
        if (tiem >= paso)
        {
            tiem -= paso;
            fra = (fra + 1) % FRAM;
        }
    }
}

void dibujar_jugador(Texture2D tex, Player *j, Color tinte)
{
    float anc = tex.width / (float)FRAM;      // 208
    float alt = tex.height / (float)FILAS;    // 158

    Rectangle ori = {fra * anc, fila_anim * alt, (j->dir < 0) ? -anc : anc, alt};

    float w = anc * ESCALA_SPRITE;
    float h = alt * ESCALA_SPRITE;
    // centrado en la caja de colisión y con los pies en su borde de abajo
    Rectangle dest = {j->rec.x + j->rec.width / 2 - w / 2,
                      j->rec.y + j->rec.height - h, w, h};

    DrawTexturePro(tex, ori, dest, Vector2{0, 0}, 0.0f, tinte);
}