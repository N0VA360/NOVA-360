# NOVA 360 - Development status

## Project target

An independent homebrew dashboard for Xbox 360 consoles using RGH.
Alternative to Aurora with a modern interface.

## Development environment

- Source repository: `N0VA360/NOVA-360`
- Main development device: Android
- Planned build environment: Linux with a verified Xbox 360 toolchain (xsdk)
- UI: Dear ImGui via `xenosworks/imgui-xenon` (xsdk GPU backend)

## Current milestone

**Dear ImGui shell implemented**

- Full-screen ImGui host window
- Sidebar navigation with the 7 planned sections:
  - Inicio (Home)
  - Biblioteca (Library)
  - Explorador (Explorer)
  - Sistema (System)
  - Personalizacion (Customize)
  - Perfil (Profile)
  - Multimedia (Media)
- Gamepad navigation enabled (`ImGuiConfigFlags_NavEnableGamepad`)
- Back button exits to the launching dashboard
- Dark teal NOVA theme

## Next milestones

1. Library: scan HDD/USB for games, show covers / titles, launch .xex
2. File explorer: browse devices, launch titles, basic file ops
3. System page: storage, free memory, temperature sensors
4. Customize: themes, background images, accent colors, sounds
5. Profile: local gamertag / avatar / simple stats
6. Multimedia: basic media browser / player hooks
7. Recent games + quick launch on Home

## Important

No compiled XEX is guaranteed until the GitHub Actions workflow finishes successfully.
The first usable target is the ImGui shell; features are filled section by section.

## Multi-AI collaboration note

This project is being developed with multiple AIs (Grok, ChatGPT, Claude, etc.).
Keep changes focused, document decisions in this file or in PRs, and avoid conflicting edits to the same files at the same time.
