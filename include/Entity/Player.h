#ifndef __PLAYER__
#define __PLAYER__

#include "Entity/Inventory/Inventory.h"
#include "Entity/Monster.h"
#include "Map/Map.h"
#include "Entity/Inventory/Potions/Potion.h"


typedef struct {
    Potion* regeneration;
    Potion* accuracy;
    Potion* experience;
    Equipment* weapon;
    Equipment* magic_wand;
    Equipment* armor;
} Bonus;

typedef struct {
    double x;
    double y;
} Vector;

typedef enum {
    PHYSICAL, MAGICAL
} AttackType;

typedef struct Player{
    AttackType attackType;

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
 * @brief Get the atk object
 * 
 * @param self 
 * @return int 
 */
int get_atk(Player* self);

/**
 * @brief Get the def object
 * 
 * @param self 
 * @return int 
 */
int get_def(Player* self);

/**
 * @brief Get the intel object
 * 
 * @param self 
 * @return int 
 */
int get_intel(Player* self);

/**
 * @brief Get the crit object
 * 
 * @param self 
 * @return int 
 */
int get_crit(Player* self);

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
 * @brief Effectue une attaque en fonction du type selectionné
 * 
 * @param self 
 * @param monster
 * @return bool
 */
bool attack_monster(Player* self, Monster* monster);

/**
 * @brief Effectue une attaque sur le player
 * 
 * @param monster 
 * @param player 
 */
void attack_player(Monster* monster, Player* player);

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

/**
 * @brief Applique les effets des potions
 * 
 * @param player 
 */
void update_bonus(Player* player);

/**
 * @brief Permet d'équiper automatiquement un équipement au joueur
 * 
 * @param player 
 */
void auto_equip(Player* player);

/**
 * @brief Vérifie si un équipement est équipé
 * 
 * @param player 
 * @param item 
 * @return true 
 * @return false 
 */
bool verif_equiped(Player* player, Item* item);



#endif