#pragma once

#include "gui/window.h"
#include "core/poly_list.h"
#include "core/edit_params.h"

#define PXLUV_COLOR_NORMAL (Color){ 0, 240, 0, 255 }
#define PXLUV_COLOR_NORMAL_BG (Color){ 100, 240, 100, 100 }
#define PXLUV_COLOR_SELECTED (Color){ 0, 0, 240, 255 }
#define PXLUV_COLOR_SELECTED_BG (Color){ 100, 100, 240, 100 }

typedef enum {
    PXLUV_EDIT_WINDOW_FG,
    PXLUV_EDIT_WINDOW_BG
} pxluv_edit_window_color_type_t;

typedef enum {
    PXLUV_EDIT_WINDOW_XY,
    PXLUV_EDIT_WINDOW_UV
} pxluv_edit_window_type_t;

typedef struct pxluv_edit_window {
    pxluv_window_t base;
    pxluv_edit_window_type_t type;
    bool add_poly_mode;
    Vector2 poly_begin;
} pxluv_edit_window_t;

pxluv_edit_window_t pxluv_edit_window_create(const char* title, Rectangle bounds, Shader shader, pxluv_edit_window_type_t type);

void pxluv_edit_window_do(
    pxluv_edit_window_t *window, 
    const Texture *textures, 
    int textures_count, 
    pxluv_poly_list_t *poly_list,
    pxluv_edit_params_t *edit_params,
    char** loaded_texture
);