#ifndef __GAMESTATES__
#define __GAMESTATES__

#include "Util/ArrayList.h"
#include "Map/Map.h"
#include "Entity/Player.h"

typedef struct {
    ArrayList* maps; /* ArrayList<Map> */
    int current_stage;
    Player player;
} GameStates;

/**
 * @brief Initialise GameStates
 * 
 * @param gs 
 */
void init_game_states(GameStates* gs);

/**
 * @brief Libère la mémoire allouée par les variables du jeu
 * 
 * @param gs 
 */
void destroy_game_states(GameStates* gs);

/**
 * @brief Get the player object
 * 
 * @param gs 
 * @return Player* 
 */
Player* get_player(GameStates* gs);

/**
 * @brief Get the current map object
 * 
 * @param gs 
 * @return Map* 
 */
Map* get_current_map(GameStates* gs);

/**
 * @brief Charge l'étage suivant
 * 
 * @param gs 
 */
void go_next_stage(GameStates* gs);

/**
 * @brief Charge l'étage précédent
 * 
 * @param gs 
 */
void go_previous_stage(GameStates* gs);

#endif