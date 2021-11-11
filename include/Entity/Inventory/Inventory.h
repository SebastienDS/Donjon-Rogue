#ifndef __INVENTORY__
#define __INVENTORY__

#include "Entity/Inventory/Equipments/Equipment.h"
#include "Entity/Inventory/Potions/Potion.h"

#define NB_ITEMS 12

typedef enum {
    EMPTY, EQUIPMENT, POTION
} ItemType;

typedef struct {
    ItemType type;
    union {
        Equipment* equipment;
        Potion* potion;
    };
} Item;

typedef struct {
    Item item[NB_ITEMS];
} Inventory;

#endif