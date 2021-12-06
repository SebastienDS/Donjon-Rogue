#ifndef __GRAPHICS__
#define __GRAPHICS__

#include <MLV/MLV_all.h>
#include "Core/GameStates.h"
#include "Interface/Images.h"
#include "constants.h"

/**
 * @brief Affiche le jeu
 * 
 * @param gs
 * @param images
 */
void draw_graphics(GameStates* gs, Images* images);

#endif