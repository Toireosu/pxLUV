#pragma once

#include <raylib.h>

#include "core/edit_params.h"
#include "core/poly_list.h"

typedef struct pxluv_list_editor {
    Rectangle bounds;
    int scroll_index;
} pxluv_list_editor_t;

pxluv_list_editor_t pxluv_list_editor_create(Rectangle bounds);

void pxluv_list_editor_do(pxluv_list_editor_t *editor, pxluv_poly_list_t *poly_list, pxluv_edit_params_t *params);