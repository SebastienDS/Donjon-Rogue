#ifndef __INTERFACE__
#define __INTERFACE__

#include "Core/GameStates.h"
#include <MLV/MLV_all.h>


/**
 * @brief Affiche l'interface
 * 
 * @param gs 
 * @param font
 */
void draw_interface(GameStates* gs, MLV_Font* font);

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


#endif