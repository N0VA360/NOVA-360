cmake_minimum_required(VERSION 3.25)
project(nova360 CXX)

# Dear ImGui (xenosworks/imgui-xenon, xenon branch)
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui-xenon")
if(NOT EXISTS "${IMGUI_DIR}/imgui.h")
  message(FATAL_ERROR
    "imgui-xenon not found at ${IMGUI_DIR}.\n"
    "Checkout it next to the sources, e.g. in the CI workflow or:\n"
    "  git clone --branch xenon --depth 1 https://github.com/xenosworks/imgui-xenon.git third_party/imgui-xenon")
endif()

add_library(imgui STATIC
  "${IMGUI_DIR}/imgui.cpp"
  "${IMGUI_DIR}/imgui_demo.cpp"
  "${IMGUI_DIR}/imgui_draw.cpp"
  "${IMGUI_DIR}/imgui_tables.cpp"
  "${IMGUI_DIR}/imgui_widgets.cpp")
target_include_directories(imgui PUBLIC "${IMGUI_DIR}" "${IMGUI_DIR}/backends")
target_compile_definitions(imgui PUBLIC IMGUI_DISABLE_DEFAULT_SHELL_FUNCTIONS)

add_executable(nova360
  src/main.cpp
  "${IMGUI_DIR}/backends/imgui_impl_xsdk.cpp")
target_link_libraries(nova360 PRIVATE imgui)
set_target_properties(nova360 PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS ON)
target_compile_options(nova360 PRIVATE -Wall -Wextra -Werror)

xsdk_add_xex(nova360 TITLE_ID 0x4E4F5641 NAME "NOVA 360" VERSION 0.2.0.0)
