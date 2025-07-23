#include "gui/window.h"
#include "pxluv.h"
#include "external/raygui.h"
#include "gui/globs.h"
#include <raymath.h>

pxluv_window_t pxluv_window_create(const char *title, Rectangle bounds, Shader shader) {
    pxluv_window_t window;
    window.bounds = bounds;
    window.render_bounds = (Rectangle){
        bounds.x,
        bounds.y + pxluv_window_header_height,
        bounds.width,
        bounds.height - pxluv_window_header_height
    };
    window.title = title;
    window.shader = shader;
    window.center = (Vector2){ 
        window.render_bounds.x + window.render_bounds.width / 2,
        window.render_bounds.y + window.render_bounds.height / 2
    };
    window.zoom = 7.0f;
    window.scroll_offset = (Vector2){ 0, 0 };

    return window;
}

Vector2 pxluv_window_get_zoomed_vector(pxluv_window_t *window, Vector2 v) {
    return (Vector2){ v.x * window->zoom, v.y * window->zoom };
}

Rectangle pxluv_window_get_zoomed_rectangle(pxluv_window_t *window, Rectangle rect) {
    Vector2 top_left = pxluv_window_get_zoomed_vector(window, (Vector2){ rect.x, rect.y });
    Vector2 size = pxluv_window_get_zoomed_vector(window, (Vector2){ rect.width, rect.height });

    return (Rectangle){
        top_left.x,
        top_left.y,
        size.x,
        size.y,
    };
};

static void pxluv_window_keep_in_bounds(pxluv_window_t *window, const Texture texture) {
    pxluv_log("Scroll: %f %f", window->scroll_offset.x, window->scroll_offset.y);
    
    Vector2 max_scroll = Vector2Scale((Vector2){ texture.width, texture.width}, window->zoom / 2);

    if (window->scroll_offset.x > max_scroll.x)
        window->scroll_offset.x = max_scroll.x; 
    if (window->scroll_offset.y > max_scroll.y)
        window->scroll_offset.y = max_scroll.y; 
    if (window->scroll_offset.x < -max_scroll.x)
        window->scroll_offset.x = -max_scroll.x; 
    if (window->scroll_offset.y < -max_scroll.y)
        window->scroll_offset.y = -max_scroll.y; 
}

void pxluv_window_do(pxluv_window_t *window, const Texture *textures, int textures_count) {
    if(!window) {
        pxluv_log("Window was null!");
        return;
    }
    
    static const float base_zoom_speed = 40.0f;
    float zoom_speed = window->zoom * base_zoom_speed;

    if (CheckCollisionPointRec(GetMousePosition(), window->render_bounds)) {
        float zoom_amount = GetMouseWheelMove() * GetFrameTime() * zoom_speed;
        if (fabs(zoom_amount) > 1e-6) {
            float prev_zoom = window->zoom;
            window->zoom += zoom_amount;

            window->scroll_offset = Vector2Scale(window->scroll_offset, window->zoom / prev_zoom);

        }
        
        if (window->zoom < 1.0f)
            window->zoom = 1.0f;
        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            Vector2 v = GetMouseDelta();
            window->scroll_offset.x += v.x;
            window->scroll_offset.y += v.y;
        }
    }

    pxluv_window_keep_in_bounds(window, textures[0]);

    BeginScissorMode(window->bounds.x, window->bounds.y, window->bounds.width, window->bounds.height); 

    ClearBackground(BLACK);

    Rectangle zoomed_view = pxluv_window_get_zoomed_rectangle(
        window, 
        (Rectangle){ 
            0, 
            0, 
            textures[0].width, 
            textures[0].height
        }
    );

    zoomed_view.x += window->center.x - zoomed_view.width / 2 + window->scroll_offset.x;
    zoomed_view.y += window->center.y - zoomed_view.height / 2 + window->scroll_offset.y;


    BeginShaderMode(window->shader);

    DrawTexturePro(
        textures[0],
        (Rectangle){0, 0, textures[0].width, textures[0].height},
        zoomed_view,
        (Vector2){ 0, 0 },
        0.0f,
        WHITE
    );
    
    EndShaderMode();

    DrawRectangle(window->bounds.x, window->bounds.y, window->bounds.width, pxluv_window_header_height, GRAY);
    pxluv_window_draw_border_full((Rectangle){
        window->bounds.x, window->bounds.y, window->bounds.width, pxluv_window_header_height
    });

    DrawText(
        window->title, 
        window->bounds.x + pxluv_window_default_padding, 
        window->bounds.y + pxluv_window_default_padding, 
        18, 
        BLACK
    );


    // Styling bonuds
    pxluv_window_draw_border_full(window->render_bounds);

    EndScissorMode();
}