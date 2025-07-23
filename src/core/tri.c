#include "core/tri.h"
#include <raymath.h>


Vector2 pxluv_tri_get_xy(pxluv_tex_coord_t coord) {
    return (Vector2){ coord.x, coord.y };
}

Vector3 pxluv_tri_barycentric_coordinates(Vector2 p, pxluv_tri_t tri) {
    Vector2 xy0 = pxluv_tri_get_xy(tri.c0);
    Vector2 xy1 = pxluv_tri_get_xy(tri.c1);
    Vector2 xy2 = pxluv_tri_get_xy(tri.c2);


    Vector2 v0 = Vector2Subtract(xy1, xy0);
    Vector2 v1 = Vector2Subtract(xy2, xy0);
    Vector2 v2 = Vector2Subtract(p, xy0);

    float d00 = Vector2DotProduct(v0, v0);
    float d01 = Vector2DotProduct(v0, v1);
    float d11 = Vector2DotProduct(v1, v1);
    float d20 = Vector2DotProduct(v2, v0);
    float d21 = Vector2DotProduct(v2, v1);

    float denom = d00 * d11 - d01 * d01;
    if (denom == 0.0f) return (Vector3){ -1, -1, -1 }; // Degenerate triangle

    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;

    return (Vector3){ u, v, w };
}

Vector2 pxluv_tri_get_uv(pxluv_tex_coord_t coord) {
    return (Vector2){ coord.u, coord.v };
}

Vector2 pxluv_tri_barycentric_to_world(Vector3 bary, pxluv_tri_t tri) {
    Vector2 u = pxluv_tri_get_uv(tri.c0);
    Vector2 v = pxluv_tri_get_uv(tri.c1);
    Vector2 w = pxluv_tri_get_uv(tri.c2);

    Vector2 p_u = Vector2Scale(u, bary.x);
    Vector2 p_v = Vector2Scale(v, bary.y);
    Vector2 p_w = Vector2Scale(w, bary.z);

    return Vector2Add(Vector2Add(p_u, p_v), p_w);
}