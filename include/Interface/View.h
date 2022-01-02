#ifndef __VIEW__
#define __VIEW__

#include <stdbool.h>
#include <MLV/MLV_all.h>
#include "Interface/Images.h"

typedef struct{
    MLV_Image* sword;
    MLV_Image* armor;
    MLV_Image* magic_wand;
    MLV_Image* instant_potion;
    MLV_Image* potion;
    MLV_Image* sword_mode;
    MLV_Image* magic_wand_mode;
} Inventory_icones;

typedef struct {
    Inventory_icones inventory_icones;
    Images images;
    Images textures;
    MLV_Font* font;
} View;




/**
 * @brief Initialise la View
 * 
 * @param view 
 */
void init_view(View* view);

/**
 * @brief Libère la mémoire allouée par View
 * 
 * @param view 
 */
void destroy_view(View* view);

#endif