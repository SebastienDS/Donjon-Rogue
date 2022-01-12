#ifndef __CORE__
#define __CORE__

#include "Core/GameStates.h"
#include "Core/Events.h"
#include "Interface/View.h"
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
 * @param view
 */
void draw(GameStates* gs, View* view);

/**
 * @brief test si on clique sur le bouton
 * 
 * @param button 
 * @param x 
 * @param y 
 * @return true 
 * @return false 
 */
bool test_click(Button* button, int x, int y);

#endif