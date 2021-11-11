#ifndef __EQUIPMENT__
#define __EQUIPMENT__

#include "Entity/Inventory/Equipments/Weapon.h"
#include "Entity/Inventory/Equipments/Armor.h"
#include "Entity/Inventory/Equipments/MagicWand.h"

typedef enum {
    WEAPON, ARMOR, MAGICWAND
} EquipmentType;

typedef struct {
    EquipmentType type;
    union {
        Weapon weapon;
        Armor armor;
        MagicWand magic_wand;
    };
} Equipment;

#endif