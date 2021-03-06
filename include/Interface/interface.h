#ifndef __INTERFACE__
#define __INTERFACE__

#include <MLV/MLV_all.h>
#include "Core/GameStates.h"
#include "Interface/View.h"


/**
 * @brief Affiche l'interface
 * 
 * @param gs 
 * @param view
 */
void draw_interface(GameStates* gs, View* view);

/**
 * @brief Affiche les différentes barres du jeu (points de vie, mana)
 * 
 * @param pos_x 
 * @param pos_y 
 * @param size_x 
 * @param size_y 
 * @param color 
 * @param value 
 * @param max 
 */
void draw_bar(int pos_x, int pos_y, int size_x, int size_y, MLV_Color color, int value, int max);

/**
 * @brief Affiche un bouton
 * 
 * @param button 
 * @param font 
 */
void draw_button(Button* button, MLV_Font* font);

/**
 * @brief Affiche le numéro de l'étage dans lequel se trouve le joueur
 * 
 * @param floor 
 * @param font 
 */
void print_floor(int floor, MLV_Font* font);

/**
 * @brief Affiche les actions possibles par l'utilisateur
 * 
 * @param gs 
 * @param font 
 */
void print_actions(GameStates* gs, MLV_Font* font);

/**
 * @brief Affiche la mini map de l'étage
 * 
 * @param gs
 * @param view 
 * @param start_x 
 * @param start_y 
 * @param cell_size 
 */
void draw_mini_map(GameStates* gs, View* view, int start_x, int start_y, int cell_size);



#endif