// NOVA 360 - modelo de datos puro (sin dependencias de xsdk ni de ImGui).
//
// Objetivo: que Biblioteca, Inicio y Personalizacion compartan estructuras
// simples y que esta logica se pueda probar sin la consola.
// Reglas: solo C++ basico, arreglos de tamano fijo, sin std::string/vector
// (no esta confirmado que la libstdc++ de la toolchain los soporte).
//
// ESTADO: todavia no esta incluido desde main.cpp, asi que CI no lo compila.

#pragma once

#include <cstdint>

namespace nova {

constexpr int kNameLen  = 64;
constexpr int kPathLen  = 260;
constexpr int kMaxGames = 256;

// Copia acotada y siempre terminada en '\0'.
inline void copy_text(char *dst, int cap, const char *src)
{
    if (!dst || cap <= 0)
        return;
    int i = 0;
    if (src) {
        for (; i < cap - 1 && src[i] != '\0'; ++i)
            dst[i] = src[i];
    }
    dst[i] = '\0';
}

// ---------------------------------------------------------------- Biblioteca

enum class GameKind : uint8_t { Unknown = 0, Xbox360, Arcade, Homebrew };

struct GameEntry {
    char     title[kNameLen];
    char     path[kPathLen];
    uint32_t title_id;
    GameKind kind;
    uint8_t  category;     // indice de categoria definido por la UI
    uint32_t last_played;  // segundos desde una referencia fija; 0 = nunca
};

struct Library {
    GameEntry games[kMaxGames];
    int       count;
};

inline void library_clear(Library *lib)
{
    if (lib)
        lib->count = 0;
}

// Devuelve nullptr si la biblioteca esta llena.
inline GameEntry *library_add(Library *lib, const char *title, const char *path,
                              uint32_t title_id, GameKind kind)
{
    if (!lib || lib->count < 0 || lib->count >= kMaxGames)
        return nullptr;
    GameEntry *g = &lib->games[lib->count++];
    copy_text(g->title, kNameLen, title);
    copy_text(g->path, kPathLen, path);
    g->title_id    = title_id;
    g->kind        = kind;
    g->category    = 0;
    g->last_played = 0;
    return g;
}

// Escribe en out[] los indices de los juegos jugados, del mas reciente al mas
// antiguo, hasta max_out elementos. Devuelve cuantos escribio.
inline int library_recent(const Library *lib, int *out, int max_out)
{
    if (!lib || !out || max_out <= 0)
        return 0;
    int n = 0;
    for (int i = 0; i < lib->count; ++i) {
        const uint32_t t = lib->games[i].last_played;
        if (t == 0)
            continue;
        int pos = n;
        while (pos > 0 && lib->games[out[pos - 1]].last_played < t)
            --pos;
        if (pos >= max_out)
            continue;
        const int last = (n < max_out) ? n : max_out - 1;
        for (int k = last; k > pos; --k)
            out[k] = out[k - 1];
        out[pos] = i;
        if (n < max_out)
            ++n;
    }
    return n;
}

// ------------------------------------------------------------ Personalizacion

struct Theme {
    const char *name;
    float       accent[3];
    float       window_bg[3];
    float       child_bg[3];
};

// El orden coincide con el combo "Oscuro / Claro / NOVA" de main.cpp.
inline constexpr Theme kThemes[] = {
    {"Oscuro", {0.25f, 0.55f, 0.95f}, {0.06f, 0.06f, 0.07f}, {0.09f, 0.09f, 0.10f}},
    {"Claro",  {0.10f, 0.45f, 0.85f}, {0.93f, 0.94f, 0.95f}, {0.97f, 0.97f, 0.98f}},
    {"NOVA",   {0.15f, 0.75f, 0.55f}, {0.05f, 0.07f, 0.06f}, {0.07f, 0.09f, 0.08f}},
};
inline constexpr int kThemeCount = 3;

} // namespace nova
