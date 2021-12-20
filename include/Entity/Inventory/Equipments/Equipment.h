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


/**
 * @brief Crée un nouvel equipement
 * 
 * @param type 
 * @return Equipment* 
 */
Equipment* equipment_new(EquipmentType type);

/**
 * @brief Libère la mémoire allouée par l'equipement
 * 
 * @param equipment 
 */
void equipment_free(Equipment* equipment);

/**
 * @brief Renvoie un equipement aléatoire
 * 
 * @param difficulty 
 * @return Equipment* 
 */
Equipment* get_random_equipment(int difficulty);

#endif