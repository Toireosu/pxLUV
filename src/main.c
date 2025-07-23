#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "external/raygui.h"

#include "core/pxluv.h"
#include "gui/window.h"
#include "gui/edit_window.h"
#include "core/edit_params.h"
#include "core/finalizer.h"
#include <rlgl.h>
#include "gui/list_editor.h"
#include "gui/top_bar.h"
#include "gui/globs.h"
#include "io/file_io.h"


int main(int argc, char* argv[]) {
    InitWindow(1200, 900, "PXLUV");

    pxluv_init();
    pxluv_file_io_init();

    pxluv_poly_list_t poly_list = pxluv_poly_list_create();
    pxluv_poly_list_add(
        &poly_list, 
        pxluv_quad_create(
            (Vector2){0, 0},
            (Vector2){0.5f, 0},
            (Vector2){0.5f, 0.5f},
            (Vector2){0, 0.5f}
        )   
    );

    pxluv_poly_list_add(
        &poly_list, 
        pxluv_quad_create(
            (Vector2){0.0f, 0.5f},
            (Vector2){0.5f, 0.5f},
            (Vector2){0.5f, 0.7f},
            (Vector2){0.0f, 0.7f}
        )   
    );


    pxluv_edit_params_t edit_params = { NULL, NULL };

    Rectangle work_space_bounds = {
        0, pxluv_window_header_height,
        1200, 900 - pxluv_window_header_height
    };

    pxluv_edit_window_t window_model = pxluv_edit_window_create(
        "Model", 
        (Rectangle){work_space_bounds.x, work_space_bounds.y, work_space_bounds.width / 2, work_space_bounds.height / 2}, 
        pxluv_shader_textured, 
        PXLUV_EDIT_WINDOW_XY
    );

    pxluv_edit_window_t window_texture = pxluv_edit_window_create(
        "Texture", 
        (Rectangle){work_space_bounds.x + work_space_bounds.width / 2, work_space_bounds.y, work_space_bounds.width / 2, work_space_bounds.height / 2}, 
        pxluv_shader_textured, 
        PXLUV_EDIT_WINDOW_UV
    );

    pxluv_window_t window_uv_textured = pxluv_window_create(
        "Result", 
        (Rectangle){work_space_bounds.x, work_space_bounds.y  + work_space_bounds.height / 2, work_space_bounds.width / 2, work_space_bounds.height / 2}, 
        pxluv_shader_uv_textured
    );

    pxluv_list_editor_t list_editor = pxluv_list_editor_create((Rectangle){work_space_bounds.x + work_space_bounds.width / 2, work_space_bounds.y  + work_space_bounds.height / 2, work_space_bounds.width / 2, work_space_bounds.height / 2});


    Texture model_texture = pxluv_texture_default_model;
    Texture texture_texture = pxluv_texture_default_texture;

    pxluv_finalizer_init(model_texture);

    pxluv_set_texture_texture(texture_texture);

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);


        char* model_to_be_loaded = NULL;
        pxluv_edit_window_do(&window_model, &model_texture, 1, &poly_list, &edit_params, &model_to_be_loaded);

        if (model_to_be_loaded) {
            model_texture = LoadTexture(model_to_be_loaded);
            pxluv_finalizer_init(model_texture);
        }

        char* texture_to_be_loaded = NULL;
        pxluv_edit_window_do(&window_texture, &texture_texture, 1, &poly_list, &edit_params, &texture_to_be_loaded);
        if (texture_to_be_loaded) {
            texture_texture = LoadTexture(texture_to_be_loaded);
            // pxluv_set_texture_texture(texture_texture);
            pxluv_set_texture_texture(texture_texture);
        }

        pxluv_finalizer_finalize(model_texture, &poly_list);

        // INFO: probably doesn't need to be set every frame since this is simple passing along an
        // ID pointing to data on the GPU :..:
        pxluv_file_io_params.texture = pxluv_finalizer.texture.texture;

        pxluv_window_do(&window_uv_textured, &pxluv_finalizer.texture.texture, 1);

        pxluv_list_editor_do(&list_editor, &poly_list, &edit_params);

        pxluv_top_bar_do(&pxluv_file_io_export);

        EndDrawing();
    }
}