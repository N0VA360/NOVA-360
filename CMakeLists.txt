cmake_minimum_required(VERSION 3.25)
project(nova360 C)

add_executable(nova360 src/main.c)
set_target_properties(nova360 PROPERTIES C_STANDARD 17 C_EXTENSIONS ON)
target_compile_options(nova360 PRIVATE -Wall -Wextra -Werror)

xsdk_add_xex(nova360 TITLE_ID 0x4E4F5641 NAME "NOVA 360" VERSION 0.1.0.0)
