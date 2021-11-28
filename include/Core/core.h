#ifndef __CORE__
#define __CORE__

#include "Core/GameStates.h"
#include "Core/Events.h"

/**
 * @brief Initialise les variables du jeu
 * 
 * @param gs 
 */
void init(GameStates* gs);

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
 */
void draw(GameStates* gs);

#endif