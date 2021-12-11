#ifndef __CORE__
#define __CORE__

#include "Core/GameStates.h"
#include "Core/Events.h"
#include "Interface/Images.h"


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
 */
void draw(GameStates* gs, Images* images);

#endif