#ifndef __GAMESTATES__
#define __GAMESTATES__

#include "Util/ArrayList.h"
#include "Util/LinkedList.h"
#include "Map/Map.h"
#include "Entity/Player.h"
#include <stdbool.h>

typedef struct GameStates GameStates;


typedef struct {
    int x;
    int y;
    int width;
    int height;
    char label[50];
    void(*callback)(GameStates* gs);
} Button;

typedef struct {
    bool inventory_is_open;
    Button equip;
    Button use;
    Button throw;
} Inventory_;

typedef struct GameStates {
    ArrayList* maps; /* ArrayList<Map> */
    int current_stage;
    Player player;
    LinkedList* path_to_stair; /* LinkedList<Position> */
    Inventory_ inventory;
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
 * @brief Renvoie le joueur
 * 
 * @param gs 
 * @return Player* 
 */
Player* get_player(GameStates* gs);

/**
 * @brief Renvoie la map
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

/**
 * @brief Met a jour le chemin vers lescalier
 * 
 * @param gs 
 */
void update_path_to_stair(GameStates* gs);


#endif