#include "core/tex_coord.h"

void pxluv_update_pixel(RenderTexture texture, pxluv_tex_coord_t coord) {
    // TODO: Transform coord.x, coord.y into "texture space" (* texture.texture.width, * texture.texture.height)
    // Transform coord.u, coord.v into "color space" i.e. * 255
};