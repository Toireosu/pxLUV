#include "gui/globs.h"

const int pxluv_window_header_height = 30;
const int pxluv_window_default_padding = 6;

void pxluv_window_draw_border(Rectangle bounds, int inset, int length, Color color) {
    DrawRectangleLines(
        bounds.x + inset, 
        bounds.y + inset, 
        bounds.width - length, 
        bounds.height - length, 
        color
    );
}

void pxluv_window_draw_border_full(Rectangle bounds) {
    pxluv_window_draw_border(bounds, 1, 5, LIGHTGRAY);
    pxluv_window_draw_border(bounds, 2, 5, GRAY);
    pxluv_window_draw_border(bounds, 3, 5, GRAY);
    pxluv_window_draw_border(bounds, 4, 5, DARKGRAY);
}