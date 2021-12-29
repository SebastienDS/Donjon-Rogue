#include "Interface/View.h"
#include "Interface/Images.h"
#include "constants.h"


void init_view(View* view) {
    init_images(&view->images, CELL_SIZE);
    init_images(&view->textures, TEXTURE_SIZE);
    view->font = MLV_load_font("resources/font/neon_pixel-7.ttf", 50);
}

void destroy_view(View* view) {
    free_images(&view->images);
    MLV_free_font(view->font);

}