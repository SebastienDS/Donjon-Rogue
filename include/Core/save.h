#ifndef __SAVE__
#define __SAVE__

#include "Core/GameStates.h"

/**
 * @brief Ecrit les informations de la partie dans un fichier
 * 
 * @param gs 
 * @param path
 */
void save_gamestates(GameStates* gs, char* path);

/**
 * @brief Charge la partie à partir d'un fichier 
 * 
 * @param path 
 * @return GameStates* 
 */
GameStates* load_gamestates(char* path);



#endif