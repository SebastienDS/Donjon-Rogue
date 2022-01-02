#include <MLV/MLV_all.h>
#include "constants.h"
#include "Interface/Images.h"


MLV_Image* load_image(const char* filename, int width, int height) {
    MLV_Image* image = MLV_load_image(filename);
    if (image == NULL) {
        fprintf(stderr, "Load image error: cannot load image %s\n", filename);
        exit(1);
    }
    
    MLV_resize_image(image, width, height);

    return image;
}

void init_images(Images* img, int size) {
    img->treasure.close = load_image("resources/images/treasure/chest_full_open_anim_f0.png", size, size);
    img->treasure.open = load_image("resources/images/treasure/chest_full_open_anim_f2.png", size, size);    
    img->zombie = load_image("resources/images/monster/big_zombie_idle_anim_f0.png", size, size);
    img->player = load_image("resources/images/player/wizzard_m_idle_anim_f0.png", size, size);
    img->stair = load_image("resources/images/floor_ladder.png", size, size);
    img->floor = load_image("resources/images/floor/floor_1.png", size, size);
    img->wall = load_image("resources/images/wall/wall_mid.png", size, size);    
}



void free_images(Images* img) {
    MLV_free_image(img->treasure.open);
    MLV_free_image(img->treasure.close);
    MLV_free_image(img->zombie);
    MLV_free_image(img->player);
    MLV_free_image(img->stair);
    MLV_free_image(img->floor);
    MLV_free_image(img->wall);
}