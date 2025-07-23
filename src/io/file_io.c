#include "io/file_io.h"

#include "core/pxluv.h"
#include <stdlib.h>

pxluv_file_io_params_t pxluv_file_io_params;

void pxluv_file_io_init() {
    pxluv_file_io_params.texture.id = 0;
}

void pxluv_file_io_export(const char* destination_path) {
    if (!pxluv_file_io_params.texture.id) {
        pxluv_log("No texture to be saved (?)");
        return;
    }
    
    Image image = LoadImageFromTexture(pxluv_file_io_params.texture);

    if (!ExportImage(image, destination_path)) {
        pxluv_log("Could not save uv texture.");
    }
    
    UnloadImage(image);
}