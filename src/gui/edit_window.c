#include "gui/edit_window.h"
#include "pxluv.h"
#include "core/quad.h"
#include <float.h>
#include <raymath.h>
#include <math.h>
#include "external/raygui.h"
#include "gui/globs.h"
#include "external/tinyfiledialogs.h"

pxluv_edit_window_t pxluv_edit_window_create(const char* title, Rectangle bounds, Shader shader, pxluv_edit_window_type_t type) {
    pxluv_edit_window_t window;
    window.base = pxluv_window_create(title, bounds, shader);
    window.type = type;
    window.add_poly_mode = false;
    window.poly_begin = (Vector2){0, 0};
    return window;
}

static Vector2 pxluv_get_typed_coordinates(pxluv_edit_window_t *window, pxluv_tex_coord_t coord) { 

    switch (window->type) {
        case PXLUV_EDIT_WINDOW_XY:
            return (Vector2){ coord.x, coord.y };
        case PXLUV_EDIT_WINDOW_UV:
            return (Vector2){ coord.u, coord.v };
    }

    pxluv_log("Unknown pxluv_edit_window_type_t: %d", window->type);
    return (Vector2){ 0, 0 };
}

static void pxluv_set_typed_coordinates(pxluv_edit_window_t *window, pxluv_tex_coord_t *coord, Vector2 v) { 

    switch (window->type) {
        case PXLUV_EDIT_WINDOW_XY:
            coord->x = v.x, coord->y = v.y;
            return;
        case PXLUV_EDIT_WINDOW_UV:
            coord->u = v.x, coord->v = v.y;
            return;
    }

    pxluv_log("Unknown pxluv_edit_window_type_t: %d", window->type);
}

static Vector2 pxluv_transform_vector2(pxluv_edit_window_t* window, Vector2 v, Vector2 texture_size) {
    // Ready for scaling
    v.x -= 0.5f;
    v.y -= 0.5f;

    v = pxluv_window_get_zoomed_vector(&window->base, v);
    v.x *= texture_size.x;
    v.y *= texture_size.y;

    v.x += window->base.center.x + window->base.scroll_offset.x;
    v.y += window->base.center.y + window->base.scroll_offset.y;

    return v;
}

static Vector2 pxluv_back_transform_vector2(pxluv_edit_window_t* window, Vector2 v, Vector2 texture_size) {
    v.x -= window->base.center.x + window->base.scroll_offset.x;
    v.y -= window->base.center.y + window->base.scroll_offset.y;
    
    v.x /= texture_size.x * window->base.zoom;
    v.y /= texture_size.y * window->base.zoom;
    
    v.x += 0.5f;
    v.y += 0.5f;

    return v;
}

static void pxluv_draw_coord(pxluv_edit_window_t *window, pxluv_tex_coord_t coord, Vector2 texture_size, Color color) {
    Vector2 position = pxluv_get_typed_coordinates(window, coord);

    position = pxluv_transform_vector2(window, position, texture_size);

    DrawRectangle(position.x - 3, position.y - 3, 6, 6, color);
}

static Color pxluv_get_color(pxluv_edit_window_color_type_t type, bool selected) {
    switch (type) {
        case PXLUV_EDIT_WINDOW_FG:
            return selected ? PXLUV_COLOR_SELECTED : PXLUV_COLOR_NORMAL;
        case PXLUV_EDIT_WINDOW_BG:
            return selected ? PXLUV_COLOR_SELECTED_BG : PXLUV_COLOR_NORMAL_BG;
    }

    pxluv_log("Unknown pxluv_edit_window_color_type_t: %d", type);
    return WHITE;
}

static void pxluv_edit_window_handle_quads(pxluv_poly_node_t* node, pxluv_edit_window_t *window, pxluv_edit_params_t *edit_params, Vector2 mouse_position, Vector2 texture_size) {
    pxluv_tri_t tris[2];
    pxluv_to_tris(node->data, tris);

    bool is_selected = edit_params->current_quad == &node->data;

    // Gettings current color
    Color fg = pxluv_get_color(PXLUV_EDIT_WINDOW_FG, is_selected);
    Color bg = pxluv_get_color(PXLUV_EDIT_WINDOW_BG, is_selected);

    // Handle selecting and drawing quads (as tris)
    for (int i = 0; i < sizeof(tris) / sizeof(pxluv_tri_t); i++) {
        pxluv_tri_t tri = tris[i];

        Vector2 v0 = pxluv_transform_vector2(window, pxluv_get_typed_coordinates(window, tri.c0), texture_size);
        Vector2 v1 = pxluv_transform_vector2(window, pxluv_get_typed_coordinates(window, tri.c1), texture_size);
        Vector2 v2 = pxluv_transform_vector2(window, pxluv_get_typed_coordinates(window, tri.c2), texture_size);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointTriangle(mouse_position, v0, v1, v2) && !window->add_poly_mode) {
            edit_params->current_quad = &node->data;
        }

        DrawTriangle(v0, v1, v2, bg);
        DrawTriangleLines(v0, v1, v2, fg);
    }
}

void pxluv_edit_window_do(
    pxluv_edit_window_t *window, 
    const Texture *textures, 
    int textures_count, 
    pxluv_poly_list_t *poly_list,
    pxluv_edit_params_t *edit_params,
    char** loaded_texture
) {
    *loaded_texture = NULL;
    pxluv_window_do(&window->base, textures, textures_count);

    Vector2 texture_size = { textures[0].width, textures[0].height };
    Vector2 mouse_position = GetMousePosition();
    Vector2 mouse_position_as_coord = pxluv_back_transform_vector2(window, mouse_position, texture_size);

    Rectangle bounds = window->base.bounds; 
    BeginScissorMode(bounds.x, bounds.y, bounds.width, bounds.height);

    bool mouse_in_bounds = CheckCollisionPointRec(GetMousePosition(), window->base.render_bounds); 

    if (mouse_in_bounds) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            edit_params->current_coord = NULL;
            edit_params->current_quad = NULL;
        }
    }

    if (GuiButton(
        (Rectangle){
            window->base.bounds.x + window->base.bounds.width - pxluv_window_header_height + pxluv_window_default_padding,
            window->base.bounds.y + pxluv_window_default_padding,
            pxluv_window_header_height - pxluv_window_default_padding * 2,
            pxluv_window_header_height - pxluv_window_default_padding * 2
        },
        GuiIconText(ICON_FILE_OPEN, "")
    )) {
        const char *file_formats[3] = {
            "*.png",
            "*.bmp",
            "*.jpeg",
        };

        *loaded_texture = tinyfd_openFileDialog(
            "Load image",
            "../",
            3,
            file_formats,
            "Select an image",
            false
        );

        if (*loaded_texture && **loaded_texture != '\0')
            return;
    }

    // Handle all quads
    pxluv_poly_node_t* node;
    PXLUV_POLY_LIST_FOREACH(poly_list, node) {

        if (node->data.hidden) continue;

        bool is_selected = edit_params->current_quad == &node->data;

        // Gettings current color
        Color fg = pxluv_get_color(PXLUV_EDIT_WINDOW_FG, is_selected);
        Color bg = pxluv_get_color(PXLUV_EDIT_WINDOW_BG, is_selected);

        pxluv_edit_window_handle_quads(node, window, edit_params, mouse_position, texture_size);

        pxluv_tex_coord_t *coords[4];
        pxluv_quad_get_coord_array(&node->data, coords);

        // Drawing coordinates
        for (int i = 0; i < sizeof(coords) / sizeof(pxluv_tex_coord_t*); i++) {
            pxluv_draw_coord(window, *coords[i], texture_size, edit_params->current_coord == coords[i] ? (Color){255, 255, 0, 255} : fg);
        }

        if (mouse_in_bounds) {

            Vector2 mouse_move = GetMouseDelta();
            mouse_move.x /= texture_size.x * window->base.zoom;
            mouse_move.y /= texture_size.y * window->base.zoom;

            // Handle Mouse Coords and Moving Coords
            if (edit_params->current_quad == &node->data) {
    
                const float max_accept_dist = 0.05f / window->base.zoom;
                float min_dist = FLT_MAX;
                pxluv_tex_coord_t *closest_coord = NULL;
    
                for (int i = 0; i < sizeof(coords) / sizeof(pxluv_tex_coord_t*); i++) {
                    
                    float dist = Vector2LengthSqr(Vector2Subtract(pxluv_get_typed_coordinates(window, *coords[i]), mouse_position_as_coord));
                    if (dist < min_dist && dist < max_accept_dist) {
                        min_dist = dist;
                        closest_coord = coords[i];
                    }
                }
            
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    edit_params->current_coord = closest_coord;
                }
    
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !Vector2Equals(mouse_move, (Vector2){0, 0})) {
                    if (edit_params->current_coord) {
                        pxluv_set_typed_coordinates(
                            window, 
                            edit_params->current_coord, 
                            Vector2Add(
                                pxluv_get_typed_coordinates(window, *edit_params->current_coord),
                                mouse_move
                            )
                        );
                    } else {
                        for (int i = 0; i < sizeof(coords) / sizeof(pxluv_tex_coord_t*); i++) {
                            pxluv_set_typed_coordinates(
                                window, 
                                coords[i], 
                                Vector2Add(
                                    pxluv_get_typed_coordinates(window, *coords[i]),
                                    mouse_move
                                )
                            );
                        }
                    }
                }
            }
        } 
        
    }
    
    if (mouse_in_bounds && !edit_params->current_quad) {
        if (window->add_poly_mode) {
            float x0 = window->poly_begin.x;
            float y0 = window->poly_begin.y;
            float x1 = mouse_position_as_coord.x;
            float y1 = mouse_position_as_coord.y;

            float min_x = fmin(x0, x1);
            float max_x = fmax(x0, x1);
            
            float min_y = fmin(y0, y1);
            float max_y = fmax(y0, y1);
            
            if (IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
                window->add_poly_mode = false;

                if ((max_x - min_x) * (max_y - min_y) > 0.005f / window->base.zoom) {
                    pxluv_poly_list_add(
                        poly_list, 
                        pxluv_quad_create(
                            (Vector2){min_x, min_y},
                            (Vector2){max_x, min_y},
                            (Vector2){max_x, max_y},
                            (Vector2){min_x, max_y}
                        )   
                    );
                }
            }

            Vector2 min_v = pxluv_transform_vector2(window, (Vector2){ min_x, min_y }, texture_size);
            Vector2 max_v = pxluv_transform_vector2(window, (Vector2){ max_x, max_y }, texture_size);

            DrawRectangle(min_v.x, min_v.y, max_v.x - min_v.x, max_v.y - min_v.y, PXLUV_COLOR_SELECTED_BG);

        } else {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                window->add_poly_mode = true;
                window->poly_begin = mouse_position_as_coord;
            }
        }
    }

    EndScissorMode();
}