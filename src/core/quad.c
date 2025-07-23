#include "core/quad.h"

static void pxluv_quad_set_coord(pxluv_tex_coord_t *coord, Vector2 v) {
    coord->x = v.x;
    coord->y = v.y;
    coord->u = v.x;
    coord->v = v.y;
}

pxluv_quad_t pxluv_quad_create(Vector2 v0, Vector2 v1, Vector2 v2, Vector2 v3) {
    pxluv_quad_t quad;
    pxluv_quad_set_coord(&quad.c0, v0);
    pxluv_quad_set_coord(&quad.c1, v1);
    pxluv_quad_set_coord(&quad.c2, v2);
    pxluv_quad_set_coord(&quad.c3, v3);

    quad.hidden = false;

    return quad;
}

void pxluv_to_tris(pxluv_quad_t quad, pxluv_tri_t* out_tris) {
    // TODO: Split quad in the middle, output two tris

    out_tris[0].c0 = quad.c0;
    out_tris[0].c1 = quad.c3;
    out_tris[0].c2 = quad.c1;

    out_tris[1].c0 = quad.c1;
    out_tris[1].c1 = quad.c3;
    out_tris[1].c2 = quad.c2;
}

void pxluv_quad_get_coord_array(pxluv_quad_t *quad, pxluv_tex_coord_t **coords) {
    coords[0] = &quad->c0;
    coords[1] = &quad->c1;
    coords[2] = &quad->c2;
    coords[3] = &quad->c3;
}