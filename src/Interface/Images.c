#include <MLV/MLV_all.h>
#include "constants.h"
#include "Interface/Images.h"


static MLV_Image* load_image(const char* filename, int width, int height) {
    MLV_Image* image = MLV_load_image(filename);
    if (image == NULL) {
        fprintf(stderr, "Load image error: cannot load image %s\n", filename);
        exit(1);
    }
    
    MLV_resize_image(image, width, height);

    return image;
}

void init_images(Images* img) {
    img->treasure.close = load_image("resources/images/treasure/chest_full_open_anim_f0.png", CELL_SIZE, CELL_SIZE);
    img->treasure.open = load_image("resources/images/treasure/chest_full_open_anim_f2.png", CELL_SIZE, CELL_SIZE);    
}



void free_images(Images* img) {
    MLV_free_image(img->treasure.open);
    MLV_free_image(img->treasure.close);
}