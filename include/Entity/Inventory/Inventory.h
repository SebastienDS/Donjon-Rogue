#ifndef __INVENTORY__
#define __INVENTORY__

#include "Entity/Inventory/Equipments/Equipment.h"
#include "Entity/Inventory/Potions/Potion.h"

#define NB_ITEMS 12

typedef enum {
    EQUIPMENT, POTION
} ItemType;

typedef struct {
    ItemType type;
    union {
        Equipment* equipment;
        Potion* potion;
    };
} Item;

typedef struct {
    Item* item[NB_ITEMS];
} Inventory;


/**
 * @brief Initialize un inventaire vide
 * 
 * @param inventory 
 */
void init_inventory(Inventory* inventory);

/**
 * @brief Crée un objet
 * 
 * @return Item* 
 */
Item* item_new();

/**
 * @brief Libère la mémoire allouée pour l'objet
 * 
 * @param item 
 */
void item_free(Item* item);

/**
 * @brief Renvoie un objet aléatoire
 * 
 * @param difficulty 
 * @return Item* 
 */
Item* get_random_item(int difficulty);

#endif