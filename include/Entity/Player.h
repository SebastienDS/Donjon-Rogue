#ifndef __PLAYER__
#define __PLAYER__

#include "Entity/Inventory/Potions/Potion.h"
#include "Entity/Inventory/Inventory.h"

#define NB_POTIONS 3

/* 
    [0] -> regeneration 
    [1] -> accuracy
    [2] -> experience
*/

typedef struct {
    Potion* potions[NB_POTIONS];
} Bonus;

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    int hp;
    int mp;
    int atk;
    int intel;
    int def;
    int exp;
    int lvl;
    int crit;
    Inventory inventory;
    Bonus bonus;
    Position pos;
} Player;

/**
 * @brief Initialise le joueur
 * 
 * @param self 
 */
void init_player(Player* self, int x, int y);

#endif