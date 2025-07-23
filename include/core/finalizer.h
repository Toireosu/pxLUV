#pragma once

#include <raylib.h>
#include "core/edit_params.h"
#include "core/poly_list.h"

typedef struct {
    RenderTexture texture;
} pxluv_finalizer_t;

extern pxluv_finalizer_t pxluv_finalizer;

void pxluv_finalizer_init(Texture original_texture);

void pxluv_finalizer_finalize(Texture original_texture, pxluv_poly_list_t* poly_list);