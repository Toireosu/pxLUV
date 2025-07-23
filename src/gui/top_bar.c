#include "gui/top_bar.h"
#include "gui/globs.h"
#include <raylib.h>
#include "external/raygui.h"
#include <stddef.h>
#include "external/tinyfiledialogs.h"

void pxluv_top_bar_do(void (*export_function)(const char*)) {
    Rectangle pxluv_top_bar_bounds = (Rectangle){
        0, 0, 1200, pxluv_window_header_height
    };

    BeginScissorMode(0, 0, 1200, pxluv_window_header_height);
    ClearBackground(GRAY);

    pxluv_window_draw_border_full(pxluv_top_bar_bounds);

    if (GuiButton(
        (Rectangle){
            pxluv_window_default_padding,
            pxluv_window_default_padding,
            100, 
            pxluv_window_header_height - pxluv_window_default_padding * 2,
        },
        "Export"
    )) {
        const char* format = "*.png"; 

        char* save_path = tinyfd_saveFileDialog(
            "Export file",
            "../uvd.png",
            0,
            &format,
            "image"
        );

        if (save_path)
            export_function(save_path);
    }

    EndScissorMode();
}