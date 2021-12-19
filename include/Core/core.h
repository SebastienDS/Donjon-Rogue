#ifndef __CORE__
#define __CORE__

#include "Core/GameStates.h"
#include "Core/Events.h"
#include "Interface/Images.h"
#include <MLV/MLV_all.h>



/**
 * @brief Update les variables du jeu
 * 
 * @param gs 
 * @param events 
 */
void update(GameStates* gs, Events* events);

/**
 * @brief Affiche le jeu
 * 
 * @param gs
 * @param images
 * @param font
 */
void draw(GameStates* gs, Images* images, MLV_Font* font);

#endif