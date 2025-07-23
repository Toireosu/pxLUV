#pragma once

#include "core/tex_coord.h"
#include "core/quad.h"

typedef struct pxluv_edit_params {
    pxluv_tex_coord_t *current_coord;
    pxluv_quad_t *current_quad;
} pxluv_edit_params_t;