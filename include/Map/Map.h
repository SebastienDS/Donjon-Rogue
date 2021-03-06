#ifndef __MAP__
#define __MAP__

#include <stdbool.h>
#include "Entity/Monster.h"
#include "Entity/Treasure.h"
#include "Util/ArrayList.h"
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
    Cell* stair_down;
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
 * @brief Crée et génere une map aléatoire
 * 
 * @return Map* 
 */
Map* map_new();

/**
 * @brief Libère la mémoire d'une map
 * 
 * @param map 
 */
void map_free(Map* map);

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
 * @brief Renvoie le nombre de voisin du type recherché
 * 
 * @param map 
 * @param cell 
 * @param neighbors 
 * @param length 
 * @param type 
 * @return int 
 */
int compute_neighbors(Map* map, Cell* cell, const int neighbors[][2], int length, Celltype type);


/**
 * @brief Génère un étage aléatoire 
 * 
 * @param map 
 * @param difficulty
 * @param player_level
 */
void generate_stage(Map* map, int difficulty, int player_level);

/**
 * @brief Renvoie si l'on peut marcher sur la cellule (x, y)
 * 
 * @param map 
 * @param x 
 * @param y 
 * @return true 
 * @return false 
 */
bool is_walkable(Map* map, int x, int y);

/**
 * @brief Renvoie les cellules voisines du type choisi
 * 
 * @param map 
 * @param cell 
 * @param type 
 * @return ArrayList* ArrayList<Cell*>
 */
ArrayList* find_neighbors(Map* map, Cell* cell, Celltype type);

/**
 * @brief Met à jour la position de l'escalier descendant
 * 
 * @param map 
 */
void update_stair_down(Map* map);


#endif