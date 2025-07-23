#pragma once
#include <raylib.h>

extern const int pxluv_window_header_height;
extern const int pxluv_window_default_padding;

void pxluv_window_draw_border(Rectangle bounds, int inset, int length, Color color);

void pxluv_window_draw_border_full(Rectangle bounds);
