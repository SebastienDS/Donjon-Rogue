#ifndef __MAP__
#define __MAP__

#include <stdbool.h>
#include "Entity/Monster.h"
#include "Entity/Treasure.h"
#include "Entity/Player.h"
#include "constants.h"

typedef enum {
    WALL, ROOM, MONSTER, TREASURE, STAIR_UP, STAIR_DOWN
} Celltype;

typedef struct {
    Celltype type;
    union {
        Monster monster;
        Treasure treasure;
    };
    int x;
    int y;
} Cell;

typedef struct {
    Cell map[HEIGHT][WIDTH];
} Map;

/**
 * @brief Renvoie la cellule à la position (x, y)
 * 
 * @param map 
 * @param x 
 * @param y 
 * @return Cell* 
 */
Cell* get_cell(Map* map, int x, int y);

/**
 * @brief Initialise la carte avec des murs et un escalier montant au centre
 * 
 * @param self map à initialiser
 */
void init_map(Map* self);

/**
 * @brief Renvoie si une cellule est sur la grille
 * 
 * @param x 
 * @param y 
 * @return true 
 * @return false 
 */
bool is_on_the_grid(int x, int y);

/**
 * @brief Renvoie si une cellule est sur le bord de la map
 * 
 * @param x 
 * @param y 
 * @return true 
 * @return false 
 */
bool is_at_the_border(int x, int y);

/**
 * @brief Génère un étage aléatoire 
 * 
 * @param map 
 */
void generate_stage(Map* map);

/**
 * @brief Renvoie si le joueur à réussie à se déplacer dans la direction (dx, dy)
 * 
 * @param map 
 * @param player 
 * @param dx 
 * @param dy 
 * @return true 
 * @return false 
 */
bool try_move(Map* map, Player* player, int dx, int dy);


#endif