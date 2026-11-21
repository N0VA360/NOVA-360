# NOVA 360 — Arquitectura y reglas de colaboración

Documento pensado para que varias IAs (y personas) trabajen en el repo sin pisarse.

## Estado a 21-nov-2026

- `src/main.cpp`: shell con Dear ImGui y las 7 secciones (Inicio, Biblioteca, Explorador, Sistema, Personalización, Perfil, Multimedia). Todas son placeholders.
- No hay XEX compilado ni probado en consola. Hay que confirmar que el workflow de CI termina en verde.

## Estructura propuesta

```
src/main.cpp          bucle principal e inicialización (solo orquesta)
src/nova/model.h      datos puros: biblioteca, temas (sin xsdk ni ImGui)
src/platform/         envoltorios de xsdk: almacenamiento, sistema, entrada
src/ui/<seccion>.cpp  una pantalla por archivo (home, library, explorer, ...)
docs/                 estado, arquitectura, notas de pruebas en consola
```

Idea: la UI no llama al kernel directamente. Pide datos a `platform/` y los guarda en las estructuras de `model.h`.

## Reglas de colaboración

1. Una rama por tarea e IA (`grok/...`, `claude/...`) y PR hacia `main`.
2. Un archivo por sección de UI. Dos IAs no editan el mismo archivo a la vez.
3. No inventar APIs: cada función de xsdk o del kernel que se use debe existir en `xenosworks/xsdk`. Indicar el header en el PR.
4. El build usa `-Werror`: no se une nada sin CI en verde.
5. Todo cambio en `CMakeLists.txt` o en el workflow se explica en el PR.

## Qué verificar antes de implementar cada sección

| Sección | Pendiente de confirmar en xsdk / kernel |
|---|---|
| Sistema | API para temperatura, memoria libre y espacio en disco |
| Explorador | Montaje de HDD/USB y listado de directorios |
| Biblioteca | Lectura de título, Title ID y carátula desde los archivos del juego |
| Perfil | Lectura del gamertag y del avatar del perfil local |
| Personalización | Dónde y cómo guardar la configuración en disco |
| Multimedia | Códecs o reproducción disponibles en la toolchain |

## Observaciones de la revisión

- `src/main.c` ya no se compila (CMake usa `main.cpp`). Conviene borrarlo para no confundir.
- La librería `imgui` compila `imgui_demo.cpp`, que agranda el XEX. Se puede quitar cuando no se use.
- `main.cpp` usa `ImGuiChildFlags_Borders` y `style.FontSizeBase`, que son de versiones recientes de ImGui. El primer build en CI dirá si la rama `xenon` de imgui-xenon las soporta.
- `docs/DEVELOPMENT_STATUS.md` quedó desactualizado respecto al estado real.
- `src/nova/model.h` todavía no está incluido desde `main.cpp`, así que CI no lo compila.
