#ifndef __MAP__
#define __MAP__

#include <stdbool.h>
#include "Entity/Monster.h"
#include "Entity/Treasure.h"

#define WIDTH 63
#define HEIGHT 43



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
 * @brief Initialise la carte avec des murs et un escalier montant au centre
 * 
 * @param self map à initialiser
 */
void init_map(Map* self);

/**
 * @brief Test l'éligibilité de la case à devenir une case "ROOM"
 * 
 * @param map 
 * @param cell
 * @return true 
 * @return false 
 */
bool is_eligible(Map* map, Cell* cell);

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


#endif