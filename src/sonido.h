// ======================================================================
//  sonido.h  -  Música y efectos con raylib (header-only, C++17)
// ======================================================================
//
//  CÓMO USARLO (solo necesitas esto):
//
//  1) En el main:
//        sonido_iniciar();       // después de InitWindow (carga todo el catálogo)
//        sonido_actualizar();    // UNA vez por frame, dentro del while
//        sonido_cerrar();        // antes de CloseWindow
//
//  2) En cualquier parte del juego, usa el NOMBRE que le pusiste en el catálogo:
//        musica_tocar("castillo");     // cambia a esa música (en loop)
//        musica_parar();               // silencio
//        musica_pausar();  musica_reanudar();
//        musica_volumen(0.5f);         // 0.0 a 1.0
//        sonido_tocar("salto");        // efecto de una sola vez
//        sonido_volumen(1.0f);         // 0.0 a 1.0
//
//  Notas:
//   - Si pides la misma música que ya suena, no se reinicia (puedes llamarla
//     cada frame sin problema). Para reiniciarla: musica_parar() y luego tocar.
//   - También puedes pasar la ruta del archivo directo: sonido_tocar("sfx/x.wav")
//   - Si un archivo no existe, el juego NO truena: sale un aviso en la consola.
//   - Formatos: .ogg .mp3 .wav .flac
//
// ======================================================================
#pragma once

#include "raylib.h"
#include <string>
#include <unordered_map>

enum TipoSonido { MUSICA, EFECTO };

struct EntradaSonido {
    const char *nombre;        // el nombre corto que usarás en el juego
    const char *archivo;       // ruta dentro de tu carpeta resources
    TipoSonido  tipo;          // MUSICA o EFECTO
    float       volumen = 1.0f;  // (opcional) volumen propio, 0.0 a 1.0
    bool        bucle   = true;  // (opcional) solo música: false = suena una vez
};

// ======================================================================
//  CATÁLOGO  -  ESTO ES LO ÚNICO QUE NECESITAS EDITAR
//  Para agregar un sonido: copia una línea, cambia nombre y archivo.
//  Para quitarlo: borra la línea. Para cambiar el archivo: cambia la ruta.
// ======================================================================
inline const EntradaSonido CATALOGO[] = {
    //  nombre        archivo                    tipo      volumen  bucle
    // ---- MÚSICA ----
    { "menu",        "musica/menu.ogg",         MUSICA },
    { "cueva",       "musica/cueva.ogg",        MUSICA },
    { "castillo",    "musica/castillo.ogg",     MUSICA },
    { "tienda",      "musica/tienda.ogg",       MUSICA, 0.6f },
    { "game_over",   "musica/game_over.ogg",    MUSICA, 1.0f, false },  // suena una vez

    // ---- EFECTOS ----
    { "salto",       "sfx/salto.wav",           EFECTO },
    { "espada",      "sfx/espada.wav",          EFECTO },
    { "moneda",      "sfx/moneda.wav",          EFECTO, 0.8f },
    { "dano",        "sfx/dano.wav",            EFECTO },
    { "muerte",      "sfx/muerte.wav",          EFECTO },
};

// ======================================================================
//  INTERNO (no hace falta tocar nada de aquí para abajo)
// ======================================================================
namespace snd {

struct Item {
    TipoSonido tipo = MUSICA;
    float volumen = 1.0f;
    bool  bucle = true;
    Music m{};
    Sound s{};
    bool  ok = false;
};

inline std::unordered_map<std::string, Item> items;
inline Item       *actual = nullptr;
inline std::string nombreActual;
inline float       volMusica  = 0.5f;
inline float       volSonidos = 1.0f;
inline bool        listo = false;

inline Item *cargar(const std::string &nombre, const std::string &archivo,
                    TipoSonido tipo, float vol, bool bucle)
{
    Item &it = items[nombre];
    it.tipo = tipo; it.volumen = vol; it.bucle = bucle;

    if (tipo == MUSICA) {
        it.m  = LoadMusicStream(archivo.c_str());
        it.ok = it.m.frameCount > 0;
        if (it.ok) { it.m.looping = bucle; SetMusicVolume(it.m, vol * volMusica); }
    } else {
        it.s  = LoadSound(archivo.c_str());
        it.ok = it.s.frameCount > 0;
    }
    if (!it.ok)
        TraceLog(LOG_WARNING, "SONIDO: no se pudo cargar '%s' (archivo: %s)",
                 nombre.c_str(), archivo.c_str());
    return &it;
}

// Busca por nombre del catálogo; si no está, lo toma como ruta de archivo.
inline Item *buscar(const std::string &nombre, TipoSonido tipo)
{
    auto f = items.find(nombre);
    Item *it = (f != items.end()) ? &f->second
                                  : cargar(nombre, nombre, tipo, 1.0f, tipo == MUSICA);
    if (it->tipo != tipo) {
        TraceLog(LOG_WARNING, "SONIDO: '%s' es %s, no %s", nombre.c_str(),
                 it->tipo == MUSICA ? "MUSICA" : "EFECTO",
                 tipo == MUSICA ? "MUSICA" : "EFECTO");
        return nullptr;
    }
    return it->ok ? it : nullptr;
}

} // namespace snd

// ======================================================================
//  FUNCIONES
// ======================================================================

inline void sonido_iniciar()
{
    if (snd::listo) return;
    InitAudioDevice();
    snd::listo = true;
    for (const EntradaSonido &e : CATALOGO)       // precarga todo el catálogo
        snd::cargar(e.nombre, e.archivo, e.tipo, e.volumen, e.bucle);
}

// Una vez por frame: mantiene la música avanzando
inline void sonido_actualizar()
{
    if (snd::actual) UpdateMusicStream(snd::actual->m);
}

inline void sonido_cerrar()
{
    if (!snd::listo) return;
    if (snd::actual) StopMusicStream(snd::actual->m);
    for (auto &p : snd::items) {
        if (!p.second.ok) continue;
        if (p.second.tipo == MUSICA) UnloadMusicStream(p.second.m);
        else                         UnloadSound(p.second.s);
    }
    snd::items.clear();
    snd::actual = nullptr;
    snd::nombreActual.clear();
    CloseAudioDevice();
    snd::listo = false;
}

// ---------------------------- MÚSICA ----------------------------------

// Toca la música. Si ya suena esa, no hace nada. Si sonaba otra, la cambia.
inline void musica_tocar(const char *nombre)
{
    if (!snd::listo || snd::nombreActual == nombre) return;
    snd::Item *it = snd::buscar(nombre, MUSICA);
    if (!it) return;
    if (snd::actual) StopMusicStream(snd::actual->m);
    snd::actual = it;
    snd::nombreActual = nombre;
    PlayMusicStream(it->m);
}

inline void musica_parar()
{
    if (snd::actual) StopMusicStream(snd::actual->m);
    snd::actual = nullptr;
    snd::nombreActual.clear();
}

inline void musica_pausar()   { if (snd::actual) PauseMusicStream(snd::actual->m); }
inline void musica_reanudar() { if (snd::actual) ResumeMusicStream(snd::actual->m); }

// 0.0 a 1.0 (por defecto 0.5)
inline void musica_volumen(float v)
{
    snd::volMusica = v;
    for (auto &p : snd::items)
        if (p.second.ok && p.second.tipo == MUSICA)
            SetMusicVolume(p.second.m, p.second.volumen * v);
}

// --------------------------- EFECTOS ----------------------------------

// Toca el efecto una vez (si ya sonaba, lo reinicia)
inline void sonido_tocar(const char *nombre)
{
    if (!snd::listo) return;
    snd::Item *it = snd::buscar(nombre, EFECTO);
    if (!it) return;
    SetSoundVolume(it->s, it->volumen * snd::volSonidos);
    PlaySound(it->s);
}

// 0.0 a 1.0 (por defecto 1.0)
inline void sonido_volumen(float v) { snd::volSonidos = v; }