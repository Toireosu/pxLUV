#pragma once

#include "core/tex_coord.h"

typedef struct plxuv_tri {
    pxluv_tex_coord_t c0, c1, c2;
} pxluv_tri_t;

Vector2 pxluv_tri_get_xy(pxluv_tex_coord_t coord);
Vector2 pxluv_tri_get_uv(pxluv_tex_coord_t coord);

Vector3 pxluv_tri_barycentric_coordinates(Vector2 p, pxluv_tri_t tri);

Vector2 pxluv_tri_barycentric_to_world(Vector3 bary, pxluv_tri_t tri);