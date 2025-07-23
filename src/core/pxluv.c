#include "core/pxluv.h"
#include <stdio.h>
#include "rlgl.h"
#include "external/raygui.h"

Shader pxluv_shader_white;
Shader pxluv_shader_textured;
Shader pxluv_shader_uv_textured;

Texture pxluv_texture_default_model;
Texture pxluv_texture_default_texture;

static void pxluv_load_textures_default() {
    pxluv_log("Loading default textures.");
    static const int TEXTURE_SIZE = 16;

    // Generating 'model' texture
    Image pxluv_image_default_model = GenImageColor(TEXTURE_SIZE, TEXTURE_SIZE, (Color){0, 0, 0, 0});

    for (int y = 0; y < TEXTURE_SIZE; y++) {
        for (int x = 0; x < TEXTURE_SIZE; x++) {
            if (x < y)
                ImageDrawPixel(&pxluv_image_default_model, x, y, WHITE);
        }
    }

    pxluv_texture_default_model = LoadTextureFromImage(pxluv_image_default_model);

    UnloadImage(pxluv_image_default_model);

    Image pxluv_image_default_texture = GenImagePerlinNoise(TEXTURE_SIZE, TEXTURE_SIZE, 0, 0, 1.0f);

    for (int y = 0; y < TEXTURE_SIZE; y++) {
        for (int x = 0; x < TEXTURE_SIZE; x++) {
            if (y >= TEXTURE_SIZE / 2) {
                if (x >= TEXTURE_SIZE / 2)
                    ImageDrawPixel(&pxluv_image_default_texture, x, y, GREEN);
                else
                    ImageDrawPixel(&pxluv_image_default_texture, x, y, BLUE);
            }
            else if (x >= TEXTURE_SIZE / 2)
                ImageDrawPixel(&pxluv_image_default_texture, x, y, PURPLE);
        }
    }

    pxluv_texture_default_texture = LoadTextureFromImage(pxluv_image_default_texture);

    UnloadImage(pxluv_image_default_texture);
}

void pxluv_set_texture_texture(Texture texture) {
    BeginShaderMode(pxluv_shader_uv_textured);

    rlActiveTextureSlot(1);
    rlEnableTexture(texture.id);

    int loc = GetShaderLocation(pxluv_shader_uv_textured, "texture1");

    // Set the uniform to use texture slot 1
    SetShaderValue(pxluv_shader_uv_textured, loc, (int[1]){1}, SHADER_UNIFORM_INT);

    rlActiveTextureSlot(0);

    EndShaderMode();
}

static void pxluv_load_shaders() {
    const char *shader_vert_code = 
    "#version 330                       \n"
    "in vec3 vertexPosition;            \n"
    "in vec2 vertexTexCoord;            \n"
    "in vec4 vertexColor;               \n"
    "out vec2 fragTexCoord;             \n"
    "out vec4 fragColor;                \n"
    "uniform mat4 mvp;                  \n"
    "void main()                        \n"
    "{                                  \n"
    "    fragTexCoord = vertexTexCoord; \n"
    "    fragColor = vertexColor;       \n"
    "    gl_Position = mvp*vec4(vertexPosition, 1.0); \n"
    "}                                  \n";

    const char* shader_white_frag_code =
    "#version 330       \n"
    "in vec2 fragTexCoord;              \n"
    "in vec4 fragColor;                 \n"
    "out vec4 finalColor;               \n"
    "uniform sampler2D texture0;        \n"
    "uniform vec4 colDiffuse;           \n"
    "void main()                        \n"
    "{                                  \n"
    "    vec4 texelColor = texture(texture0, fragTexCoord);   \n"
    "    vec4 colors[2] = vec4[2](       \n"
    "        vec4(0.0, 0.0, 0.0, 0.0),  \n"
    "        vec4(1.0, 1.0, 1.0, 1.0)  \n"
    "    );                             \n"
    "    finalColor = colors[int(texelColor.a + 0.5)];        \n"
    "}                                  \n";


    pxluv_log("Loading 'white' shader.");
    pxluv_shader_white = LoadShaderFromMemory(shader_vert_code, shader_white_frag_code);

    const char* shader_textured_frag_code =
    "#version 330       \n"
    "in vec2 fragTexCoord;              \n"
    "in vec4 fragColor;                 \n"
    "out vec4 finalColor;               \n"
    "uniform sampler2D texture0;        \n"
    "uniform vec4 colDiffuse;           \n"
    "void main()                        \n"
    "{                                  \n"
    "    vec4 texelColor = texture(texture0, fragTexCoord);   \n"
    "    finalColor = texelColor;        \n"
    "}                                  \n";

    pxluv_log("Loading 'textured' shader.");
    pxluv_shader_textured = LoadShaderFromMemory(shader_vert_code, shader_textured_frag_code);

    const char* shader_uv_textured_frag_code =
    "#version 330       \n"
    "in vec2 fragTexCoord;              \n"
    "in vec4 fragColor;                 \n"
    "out vec4 finalColor;               \n"
    "uniform sampler2D texture0;        \n"
    "uniform sampler2D texture1;        \n"
    "uniform vec4 colDiffuse;           \n"
    "void main()                        \n"
    "{                                  \n"
    "    vec4 texelColor = texture(texture0, fragTexCoord);   \n"
    "    if (texelColor.a < 0.01)              \n"
    "        discard;                   \n"
    "    finalColor = texture(texture1, texelColor.xy);        \n"
    "}                                  \n";

    pxluv_log("Loading 'uv textured' shader.");
    pxluv_shader_uv_textured = LoadShaderFromMemory(shader_vert_code, shader_uv_textured_frag_code);
}

// Initializes pxluv
void pxluv_init() {
    pxluv_log("Initializing.");
    pxluv_load_textures_default();
    pxluv_load_shaders();

    pxluv_log("Setting styles.");
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0xAAAAAAff);
    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, 0x333333ff);
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, 0xAAAAAAff);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0x000000ff);
    GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, 0xAAAAAAff);
    GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, 0xDDDDDDff);
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, 0x000000ff);
    GuiSetStyle(DEFAULT, BORDER_COLOR_PRESSED, 0x22AAAAff);
    GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, 0x22DDDDff);
    GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, 0x000000ff);
}

void pxluv_log(const char* text, ...) {
    va_list args;
    va_start(args, text);
    printf("[pxLUV]: ");
    vprintf(text, args);
    printf("\n");
    va_end(args);
}
