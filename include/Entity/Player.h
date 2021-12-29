#ifndef __PLAYER__
#define __PLAYER__

#include "Entity/Inventory/Potions/Potion.h"
#include "Entity/Inventory/Inventory.h"
#include "Entity/Monster.h"
#include "Map/Map.h"

#define NB_POTIONS 3
#define REGENERATION 0
#define ACCURACY 1
#define EXPERIENCE 2

/* 
    [0] -> regeneration 
    [1] -> accuracy
    [2] -> experience
*/

typedef struct {
    Potion* potions[NB_POTIONS];
} Bonus;

typedef struct {
    double x;
    double y;
} Vector;

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
    Vector position;

    Vector direction;
    Vector plane;
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

/**
 * @brief Renvoie si le joueur à réussie à se déplacer dans la direction (dx, dy)
 * 
 * @param player 
 * @param map 
 * @param dx 
 * @param dy 
 * @return true 
 * @return false 
 */
bool try_move(Player* player, Map* map, int dx, int dy);

/**
 * @brief Tourne le joueur de X radians
 * 
 * @param self 
 * @param radians 
 */
void rotate_player(Player* self, double radians);


#endif