#include "core/finalizer.h"
#include "core/poly_list.h"
#include <stdlib.h>
#include "core/pxluv.h"
#include <raymath.h>

pxluv_finalizer_t pxluv_finalizer;

void pxluv_finalizer_init(Texture original_texture) {
    pxluv_finalizer.texture = LoadRenderTexture(original_texture.width, original_texture.height); 
}

static Vector2 pxluv_finalizer_vector_transform(Texture texture, Vector2 v) {
    return (Vector2){ (v.x + 0.5) / texture.width, (v.y + 0.5) / texture.height };
}

static unsigned char pxluv_coord_to_color_space(float f) {
    return Clamp(f, 0.0f, 1.0f) * 255;
}

void pxluv_finalizer_finalize(Texture original_texture, pxluv_poly_list_t* poly_list) {

    // Get all tris
    int list_size = pxluv_poly_list_size(poly_list);
    int tri_list_size = 2 * list_size;
    pxluv_tri_t *tris = (pxluv_tri_t*)malloc(sizeof(pxluv_tri_t) * tri_list_size);
    pxluv_poly_node_t *node;

    int i = 0;
    PXLUV_POLY_LIST_FOREACH(poly_list, node) {
        pxluv_tri_t current_tris[2];
        pxluv_to_tris(node->data, current_tris);
        
        tris[i] = current_tris[0];
        tris[i + 1] = current_tris[1];

        i += 2;
    }

    // Copy Texture
    BeginTextureMode(pxluv_finalizer.texture);
    ClearBackground(BLANK);

    Image image = LoadImageFromTexture(original_texture);
    for (int y = 0; y < original_texture.height; y++) {
        for (int x = 0; x < original_texture.width; x++) {
            // SetTraceLogLevel(LOG_WARNING);
            Color c = GetImageColor(image, x, original_texture.height - 1 - y);
            if (c.a == 0) continue;
            // SetTraceLogLevel(LOG_ALL);

            Vector2 linear_vector = pxluv_finalizer_vector_transform(original_texture, (Vector2){ x, original_texture.height - 1 - y });

            linear_vector.x = Clamp(linear_vector.x, 0.0f, 1.0f);
            linear_vector.y = Clamp(linear_vector.y, 0.0f, 1.0f);

            // Check what tri position belongs to
            pxluv_tri_t* tri = NULL;
            for (int i = 0; i < tri_list_size; i++) {
                if (CheckCollisionPointTriangle(linear_vector, pxluv_tri_get_xy(tris[i].c0), pxluv_tri_get_xy(tris[i].c1), pxluv_tri_get_xy(tris[i].c2))) {        
                    tri = &tris[i];
                    break;
                }
            }

            if (tri == NULL) {
                DrawPixel(x, y, WHITE);
            } else {
                Vector3 bary = pxluv_tri_barycentric_coordinates(linear_vector, *tri);

                Vector2 uv = pxluv_tri_barycentric_to_world(bary, *tri);
                // Clamping

                DrawPixel(
                    x, 
                    y,
                    (Color){
                        pxluv_coord_to_color_space(uv.x),
                        pxluv_coord_to_color_space(uv.y),
                        /* UNUSED*/ 0,
                        c.a
                    }
                );
            }
        }
    }

    UnloadImage(image);

    EndTextureMode();

    free(tris);
}