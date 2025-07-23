#pragma once

#include <raylib.h>

typedef struct pxluv_tex_coord {
    float x, y;
    float u, v;
} pxluv_tex_coord_t;

void pxluv_update_pixel(RenderTexture texture, pxluv_tex_coord_t coord);