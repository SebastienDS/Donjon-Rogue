#include "Interface/View.h"
#include "Interface/Images.h"
#include "constants.h"



void init_view(View* view) {
    int size_icone_inventory = (SCREEN_WIDTH * 2 / 5 - 45) / 4;
    int size_mode_attack = 50;

    init_images(&view->images, CELL_SIZE);
    init_images(&view->textures, TEXTURE_SIZE);
    view->font = MLV_load_font("resources/font/neon_pixel-7.ttf", 50);
    view->medium_font = MLV_load_font("resources/font/neon_pixel-7.ttf", 30);

    view->inventory_icones.armor = load_image("resources/images/items/armor.png", size_icone_inventory, size_icone_inventory);
    view->inventory_icones.sword = load_image("resources/images/items/sword.png", size_icone_inventory, size_icone_inventory);
    view->inventory_icones.sword_mode = load_image("resources/images/items/sword.png", size_mode_attack, size_mode_attack);
    view->inventory_icones.magic_wand = load_image("resources/images/items/magic_wand.png", size_icone_inventory, size_icone_inventory);
    view->inventory_icones.magic_wand_mode = load_image("resources/images/items/magic_wand.png", size_mode_attack, size_mode_attack);
    view->inventory_icones.instant_potion = load_image("resources/images/items/instant_potion.png", size_icone_inventory, size_icone_inventory);
    view->inventory_icones.potion = load_image("resources/images/items/potion.png", size_icone_inventory, size_icone_inventory);
}

void destroy_view(View* view) {
    free_images(&view->images);
    MLV_free_font(view->font);
    MLV_free_font(view->medium_font);
    MLV_free_image(view->inventory_icones.armor);
    MLV_free_image(view->inventory_icones.sword);
    MLV_free_image(view->inventory_icones.sword_mode);
    MLV_free_image(view->inventory_icones.magic_wand);
    MLV_free_image(view->inventory_icones.magic_wand_mode);
    MLV_free_image(view->inventory_icones.instant_potion);
    MLV_free_image(view->inventory_icones.potion);
}