#ifndef __PLAYER__
#define __PLAYER__

#include "Entity/Inventory/Potions/Potion.h"
#include "Entity/Inventory/Inventory.h"
#include "Entity/Monster.h"

#define NB_POTIONS 3
#define REGENERATION_INDEX 0
#define ACCURACY_INDEX 1
#define EXPERIENCE_INDEX 2

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

/**
 * @brief Renvoie les hp max du player
 * 
 * @param self 
 * @return int
 */
int get_hp_player(Player* self);

/**
 * @brief Renvoie les mp max du player
 * 
 * @param self 
 * @return int
 */
int get_mp_player(Player* self);

/**
 * @brief Effectue une attaque physique
 * 
 * @param self 
 * @param monster 
 */
void physical_attack(Player* self, Monster* monster);

/**
 * @brief Effectue une attaque magique
 * 
 * @param self 
 * @param monster 
 */
void magical_attack(Player* self, Monster* monster);


#endif