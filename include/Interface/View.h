#ifndef __VIEW__
#define __VIEW__

#include <stdbool.h>
#include <MLV/MLV_all.h>
#include "Interface/Images.h"

typedef struct {
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