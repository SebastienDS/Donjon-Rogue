#ifndef __EQUIPMENT__
#define __EQUIPMENT__

#include "Entity/Equipments/Weapon.h"
#include "Entity/Equipments/Armor.h"
#include "Entity/Equipments/MagicWand.h"

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