#ifndef __GAMESTATES__
#define __GAMESTATES__

#include "Util/ArrayList.h"
#include "Util/LinkedList.h"
#include "Map/Map.h"
#include "Entity/Player.h"
#include <stdbool.h>

#define LOG_COUNT 5

typedef struct GameStates GameStates;


typedef enum {
    DEFAULT, RAYCASTING
} ViewType;

typedef struct {
    int x;
    int y;
    int width;
    int height;
    char label[50];
    void(*callback)(GameStates* gs);
} Button;

typedef struct {
    Item* item_selected;
    int index;
    bool is_open;
    Button equip;
    Button use;
    Button throw;
} InventoryInterface;

typedef struct {
    Item* item_selected;
    int index;
    bool is_open;
    Treasure* treasure;
    Cell* cell;
    Button take;
    Button close;
} TreasureInterface;

typedef struct {
    Button atk;
    Button intel;
    Button def;
} SkillsUpgrades;

typedef struct GameStates {
    ViewType viewType;

    ArrayList* maps; /* ArrayList<Map> */
    int current_stage;
    Player player;

    bool end_turn;

    InventoryInterface inventory;
    TreasureInterface treasure;
    SkillsUpgrades skills_btn;

    char* logs[LOG_COUNT];

    LinkedList* path_to_stair; /* LinkedList<PathPosition> */
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

/**
 * @brief Set the button object
 * 
 * @param button 
 * @param x 
 * @param y 
 * @param width 
 * @param height 
 * @param label 
 * @param callback 
 */
void set_button(Button* button, int x, int y, int width, int height, char label[50], void(*callback)(GameStates* gs));

/**
 * @brief Log un evenement de la partie
 * 
 * @param gs 
 * @param log 
 */
void log_event(GameStates* gs, char* log);

#endif