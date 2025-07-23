#pragma once

#include <raylib.h>

typedef struct {
    Texture texture;
} pxluv_file_io_params_t;

extern pxluv_file_io_params_t pxluv_file_io_params;

void pxluv_file_io_init();

void pxluv_file_io_export(const char* destination_path);