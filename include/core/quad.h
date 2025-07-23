#pragma once

#include "core/tex_coord.h"
#include "core/tri.h"

typedef struct pxluv_quad {
    pxluv_tex_coord_t c0, c1, c2, c3;
    bool hidden;
} pxluv_quad_t;

pxluv_quad_t pxluv_quad_create(Vector2 v0, Vector2 v1, Vector2 v3, Vector2 v4);

void pxluv_to_tris(pxluv_quad_t quad, pxluv_tri_t* out_tris);

void pxluv_quad_get_coord_array(pxluv_quad_t *quad, pxluv_tex_coord_t **coords);