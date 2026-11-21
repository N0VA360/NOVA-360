// NOVA 360 - Independent homebrew dashboard for Xbox 360 RGH
// Shell with Dear ImGui (xsdk backend). Press Back to return to the launching dashboard.

#include "imgui.h"
#include "imgui_impl_xsdk.h"

#include "xsdk/gpu.h"
#include "xsdk/kernel.h"
#include "xsdk/log.h"
#include "xsdk/present.h"
#include "xsdk/splash.h"

#include <cstdint>

enum {
    SPLASH_MS        = 1200,
    BUFFERS          = 2,
    SWAP_INTERVAL    = 1,
    BEGIN_TIMEOUT_MS = 5000,
    CLEAR_ARGB       = 0xFF0B1210,
};

enum class Section : int {
    Home = 0,
    Library,
    Explorer,
    System,
    Customize,
    Profile,
    Media,
    Count
};

static const char *section_names[] = {
    "Inicio",
    "Biblioteca",
    "Explorador",
    "Sistema",
    "Personalizacion",
    "Perfil",
    "Multimedia",
};

static Section g_section = Section::Home;

static void draw_home()
{
    ImGui::TextUnformatted("Bienvenido a NOVA 360");
    ImGui::Spacing();
    ImGui::TextWrapped(
        "Dashboard homebrew independiente para Xbox 360 RGH.\n"
        "Usa el D-pad o el stick izquierdo para navegar, A para seleccionar, B para volver.");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::TextUnformatted("Juegos recientes");
    ImGui::BeginChild("recent", ImVec2(0, 180), ImGuiChildFlags_Borders);
    ImGui::TextDisabled("(Aun no hay juegos recientes)");
    ImGui::TextDisabled("La biblioteca escaneara tus titulos aqui.");
    ImGui::EndChild();

    ImGui::Spacing();
    ImGui::TextUnformatted("Accesos rapidos");
    if (ImGui::Button("Biblioteca", ImVec2(160, 40)))
        g_section = Section::Library;
    ImGui::SameLine();
    if (ImGui::Button("Explorador", ImVec2(160, 40)))
        g_section = Section::Explorer;
    ImGui::SameLine();
    if (ImGui::Button("Sistema", ImVec2(160, 40)))
        g_section = Section::System;
}

static void draw_library()
{
    ImGui::TextUnformatted("Biblioteca de juegos");
    ImGui::TextDisabled("Caratulas, categorias y organizacion - en desarrollo.");
    ImGui::Spacing();
    ImGui::BeginChild("games", ImVec2(0, 0), ImGuiChildFlags_Borders);
    ImGui::TextDisabled("Aqui apareceran los juegos detectados en HDD / USB.");
    ImGui::EndChild();
}

static void draw_explorer()
{
    ImGui::TextUnformatted("Explorador de archivos");
    ImGui::TextDisabled("Navegacion por HDD, USB y dispositivos - en desarrollo.");
    ImGui::Spacing();
    ImGui::BeginChild("files", ImVec2(0, 0), ImGuiChildFlags_Borders);
    ImGui::TextDisabled("Lista de archivos y carpetas.");
    ImGui::EndChild();
}

static void draw_system()
{
    ImGui::TextUnformatted("Sistema");
    ImGui::Spacing();
    ImGui::Text("Almacenamiento:  (pendiente)");
    ImGui::Text("Memoria libre:   (pendiente)");
    ImGui::Text("Temperatura:     (pendiente)");
    ImGui::Spacing();
    ImGui::TextDisabled("Estos valores se leeran del kernel / sensores en una proxima iteracion.");
}

static void draw_customize()
{
    ImGui::TextUnformatted("Personalizacion");
    ImGui::TextDisabled("Fondos, temas, colores y sonidos - en desarrollo.");
    ImGui::Spacing();

    static float accent[3] = {0.15f, 0.75f, 0.55f};
    ImGui::ColorEdit3("Color de acento", accent);

    static int theme = 0;
    ImGui::Combo("Tema", &theme, "Oscuro\0Claro\0NOVA\0");
}

static void draw_profile()
{
    ImGui::TextUnformatted("Perfil");
    ImGui::TextDisabled("Avatar, gamertag y estadisticas locales - en desarrollo.");
    ImGui::Spacing();
    ImGui::Text("Gamertag:  (local)");
    ImGui::Text("Avatar:    (placeholder)");
}

static void draw_media()
{
    ImGui::TextUnformatted("Multimedia");
    ImGui::TextDisabled("Reproductor y accesos multimedia compatibles - en desarrollo.");
}

static void draw_section_content()
{
    switch (g_section) {
    case Section::Home:      draw_home();      break;
    case Section::Library:   draw_library();   break;
    case Section::Explorer:  draw_explorer();  break;
    case Section::System:    draw_system();    break;
    case Section::Customize: draw_customize(); break;
    case Section::Profile:   draw_profile();   break;
    case Section::Media:     draw_media();     break;
    default: break;
    }
}

static void draw_ui(uint32_t width, uint32_t height)
{
    const float sidebar_w = 220.0f;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)width, (float)height));
    ImGui::Begin("NOVA360", nullptr,
                 ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoBringToFrontOnFocus |
                 ImGuiWindowFlags_NoNavFocus);

    ImGui::BeginChild("sidebar", ImVec2(sidebar_w, 0), ImGuiChildFlags_Borders);
    ImGui::Spacing();
    ImGui::TextUnformatted("  NOVA 360");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    for (int i = 0; i < (int)Section::Count; i++) {
        bool selected = (g_section == (Section)i);
        if (ImGui::Selectable(section_names[i], selected, 0, ImVec2(0, 36)))
            g_section = (Section)i;
    }

    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("content", ImVec2(0, 0), ImGuiChildFlags_Borders);
    ImGui::Spacing();
    draw_section_content();
    ImGui::EndChild();

    ImGui::End();
}

int main()
{
    xsdk_log_write("NOVA 360: boot (ImGui shell)");

    const NTSTATUS splash = xsdk_splash_show(SPLASH_MS);
    if (!NT_SUCCESS(splash))
        xsdk_log_write("NOVA: xsdk_splash_show failed: 0x%08X", (unsigned)splash);

    NTSTATUS status = xsdk_gpu_open();
    if (!NT_SUCCESS(status)) {
        xsdk_log_write("NOVA: xsdk_gpu_open failed: 0x%08X", (unsigned)status);
        return 1;
    }

    uint32_t width = 0, height = 0;
    status = xsdk_present_open(XSDK_DISPLAY_720P, BUFFERS, SWAP_INTERVAL, &width, &height);
    if (!NT_SUCCESS(status)) {
        xsdk_log_write("NOVA: xsdk_present_open failed: 0x%08X", (unsigned)status);
        return 1;
    }
    xsdk_log_write("NOVA: display ready (%ux%u)", (unsigned)width, (unsigned)height);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.IniFilename = nullptr;

    ImGui::StyleColorsDark();
    ImGuiStyle &style = ImGui::GetStyle();
    style.FontSizeBase   = 22.0f;
    style.FrameRounding  = 4.0f;
    style.WindowRounding = 0.0f;
    style.ScrollbarRounding = 4.0f;
    style.GrabRounding   = 4.0f;
    style.ItemSpacing    = ImVec2(10, 8);

    ImVec4 *colors = style.Colors;
    colors[ImGuiCol_Header]         = ImVec4(0.12f, 0.55f, 0.45f, 0.70f);
    colors[ImGuiCol_HeaderHovered]  = ImVec4(0.15f, 0.70f, 0.55f, 0.85f);
    colors[ImGuiCol_HeaderActive]   = ImVec4(0.10f, 0.60f, 0.50f, 1.00f);
    colors[ImGuiCol_Button]         = ImVec4(0.12f, 0.50f, 0.42f, 0.70f);
    colors[ImGuiCol_ButtonHovered]  = ImVec4(0.15f, 0.65f, 0.52f, 0.90f);
    colors[ImGuiCol_ButtonActive]   = ImVec4(0.10f, 0.55f, 0.45f, 1.00f);
    colors[ImGuiCol_FrameBg]        = ImVec4(0.12f, 0.16f, 0.15f, 1.00f);
    colors[ImGuiCol_WindowBg]       = ImVec4(0.05f, 0.07f, 0.06f, 1.00f);
    colors[ImGuiCol_ChildBg]        = ImVec4(0.07f, 0.09f, 0.08f, 1.00f);

    if (!ImGui_ImplXsdk_Init()) {
        xsdk_log_write("NOVA: ImGui_ImplXsdk_Init failed");
        return 1;
    }

    for (;;) {
        INPUT_STATE input = {};
        if (XamInputGetState(0, 0, &input) == ERROR_SUCCESS) {
            if (input.gamepad.buttons & INPUT_GAMEPAD_BACK)
                break;
            ImGui_ImplXsdk_ProcessGamepad(&input.gamepad);
        }

        ImGui_ImplXsdk_NewFrame(width, height);
        ImGui::NewFrame();

        draw_ui(width, height);

        ImGui::Render();

        uint32_t address = 0;
        status = xsdk_present_begin(&address, BEGIN_TIMEOUT_MS);
        if (!NT_SUCCESS(status)) {
            xsdk_log_write("NOVA: xsdk_present_begin failed: 0x%08X", (unsigned)status);
            break;
        }

        xsdk_gpu_set_target(width, height);
        ImGui_ImplXsdk_RenderDrawData(ImGui::GetDrawData());
        xsdk_gpu_resolve(address, width, height, CLEAR_ARGB);
        xsdk_present_end();
    }

    ImGui_ImplXsdk_Shutdown();
    ImGui::DestroyContext();

    xsdk_present_close();
    status = xsdk_gpu_close();
    if (!NT_SUCCESS(status))
        xsdk_log_write("NOVA: xsdk_gpu_close failed: 0x%08X", (unsigned)status);

    xsdk_log_write("NOVA 360: shutdown");
    return 0;
}
