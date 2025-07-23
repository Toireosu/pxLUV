#pragma once

#include <raylib.h>

extern Shader pxluv_shader_white;
extern Shader pxluv_shader_textured;
extern Shader pxluv_shader_uv_textured;

extern Texture pxluv_texture_default_model;
extern Texture pxluv_texture_default_texture;

// Initializes pxluv
void pxluv_init();

void pxluv_log(const char* text, ...);

void pxluv_set_texture_texture(Texture texture);