#pragma once

#include <raylib.h>

typedef struct pxluv_window {
    const char* title;
    Rectangle bounds;
    Rectangle render_bounds;
    Vector2 center;
    float zoom;
    Shader shader;
    Vector2 scroll_offset;
} pxluv_window_t;

pxluv_window_t pxluv_window_create(const char* title, Rectangle bounds, Shader shader);

void pxluv_window_do(pxluv_window_t *window, const Texture *textures, int textures_count);

Vector2 pxluv_window_get_zoomed_vector(pxluv_window_t *window, Vector2 v);

Rectangle pxluv_window_get_zoomed_rectangle(pxluv_window_t *window, Rectangle rect);