#ifndef __LOAD__
#define __LOAD__

#include "Core/GameStates.h"

/**
 * @brief Charge la partie à partir d'un fichier 
 * 
 * @param path 
 * @param gs
 */
void load_gamestates(char* path, GameStates* gs);

#endif