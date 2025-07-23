cmake_minimum_required(VERSION 3.10)
project(pxluv C)

set(CMAKE_C_STANDARD 11)

find_package(raylib REQUIRED)

add_executable(pxluv 
    src/main.c
    src/core/pxluv.c
    src/gui/globs.c
    src/gui/window.c
    src/core/tex_coord.c
    src/core/tri.c
    src/core/quad.c
    src/gui/edit_window.c
    src/core/poly_list.c
    src/core/edit_params.c
    src/core/finalizer.c
    src/gui/list_editor.c
    src/external/tinyfiledialogs.c
    src/gui/top_bar.c
    src/io/file_io.c
)

target_include_directories(pxluv PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gui
    ${CMAKE_CURRENT_SOURCE_DIR}/include/core)


target_link_libraries(pxluv raylib)

find_program(CLANG_TIDY_EXE NAME clang-tidy)
if (CLANG_TIDY_EXE)
    set(CMAKE_C_CLANG_TIDY "${CLANG_TIDY_EXE}")
endif()