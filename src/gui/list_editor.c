#include "gui/list_editor.h"
#include "gui/globs.h"
#include <external/raygui.h>
#include <stdlib.h>

pxluv_list_editor_t pxluv_list_editor_create(Rectangle bounds) {
    pxluv_list_editor_t editor;
    editor.bounds = bounds;
    editor.scroll_index = 0;

    return editor;
}

void pxluv_list_editor_do(pxluv_list_editor_t *editor, pxluv_poly_list_t *poly_list, pxluv_edit_params_t *params) {

    BeginScissorMode(editor->bounds.x, editor->bounds.y, editor->bounds.width, editor->bounds.height);

    ClearBackground(GRAY);
    pxluv_window_draw_border_full((Rectangle){ editor->bounds.x, editor->bounds.y, editor->bounds.width, editor->bounds.height});


    // Should we do this every frame? probably not but it's easy
    int options_count = pxluv_poly_list_size(poly_list);
    const char** options = (const char**)malloc(options_count * sizeof(char*));

    pxluv_quad_t** quads = (pxluv_quad_t**)malloc(options_count * sizeof(pxluv_quad_t*));
    
    pxluv_poly_node_t* current;
    int i = 0;
    int selected_item = -1;
    PXLUV_POLY_LIST_FOREACH(poly_list, current) {
        quads[i] = &current->data;
        if (&current->data == params->current_quad)
            selected_item = i;

        i++;
    }


    for (i = 0; i < options_count; i++) {
        options[i] = TextFormat("Poly %d", i);
    }

    static const int list_width = 100; 

    int last_selected = selected_item;
    GuiListViewEx(
        (Rectangle){ 
            editor->bounds.x + pxluv_window_default_padding,
            editor->bounds.y + pxluv_window_default_padding,
            list_width,
            editor->bounds.height - pxluv_window_default_padding * 2
        },
        options,
        options_count,
        &editor->scroll_index,
        &selected_item,
        NULL
    );

    if (last_selected != selected_item) {
        params->current_coord = NULL; 
        params->current_quad = quads[selected_item];
    }

    if (selected_item >= 0) {
        
        GuiCheckBox(
            (Rectangle){
                editor->bounds.x + list_width + pxluv_window_default_padding * 2,
                editor->bounds.y + pxluv_window_default_padding * 2 + 30,
                30 - pxluv_window_default_padding * 2,
                30 - pxluv_window_default_padding * 2,
            },
            "Hide",
            &params->current_quad->hidden
        );

        if (params->current_quad->hidden)
            GuiSetState(STATE_DISABLED);

        if (GuiButton(
            (Rectangle){
                editor->bounds.x + list_width + pxluv_window_default_padding,
                editor->bounds.y + pxluv_window_default_padding,
                list_width,
                30,
            },
            "Delete"
        )) {
            params->current_coord = NULL;
            pxluv_poly_list_remove_index(poly_list, selected_item);
            params->current_quad = --selected_item >= 0 ? quads[selected_item] : NULL;
            
        }

        GuiSetState(STATE_NORMAL);
    }

    free(options);

    EndScissorMode();

}