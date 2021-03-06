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

    int skill_points;
    
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
 * @brief Effectue une attaque en fonction du type selectionn√©
 * 
 * @param self 
 * @param monster
 * @param type
 * @return bool
 */
bool attack_monster(Player* self, Monster* monster, AttackType type);

/**
 * @brief Effectue une attaque sur le player
 * 
 * @param monster 
 * @param player 
 */
void attack_player(Monster* monster, Player* player);

/**
 * @brief Renvoie si le joueur √† r√©ussie √† se d√©placer dans la direction (dx, dy)
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
 * @brief Permet d'√©quiper automatiquement un √©quipement au joueur
 * 
 * @param player 
 */
void auto_equip(Player* player);

/**
 * @brief V√©rifie si un √©quipement est √©quip√©
 * 
 * @param player 
 * @param item 
 * @return true 
 * @return false 
 */
bool verif_equiped(Player* player, Item* item);

/**
 * @brief Renvoie l'experience requit pour monter de niveau
 * 
 * @param level 
 * @return int 
 */
int required_experience(int level);

/**
 * @brief Augmente l'experience du joueur
 * 
 * @param player 
 * @param exp 
 */
void gain_experience(Player* player, int exp);

/**
 * @brief Get the empty slot object
 * 
 * @param player 
 * @return int 
 */
int get_empty_slot(Player* player);

/**
 * @brief Insert un item dans l'inventaire du joueur
 * 
 * @param player 
 * @param item 
 * @param index 
 * @return true 
 * @return false 
 */
bool insert_item(Player* player, Item* item, int index);

/**
 * @brief Renvoie si le joueur est mort
 * 
 * @param player 
 * @return true 
 * @return false 
 */
bool is_player_dead(Player* player);


#endif