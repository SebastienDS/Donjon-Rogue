#ifndef __TREASURE__
#define __TREASURE__

#include "Entity/Inventory/Inventory.h"
#define ITEMS_PER_TREASURE 2

typedef enum {
    OPEN, CLOSE
} State;

typedef struct {
    State state;
    Item* items[ITEMS_PER_TREASURE];
} Treasure;

/**
 * @brief Initialise un trésor
 * 
 * @param treasure 
 */
void init_treasure(Treasure* treasure, int difficulty);

/**
 * @brief Détruit le trésor
 * 
 * @param treasure 
 */
void destroy_treasure(Treasure* treasure);


#endif