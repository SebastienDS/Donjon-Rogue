#include <stdlib.h>
#include <stdio.h>

#include "Entity/Inventory/Equipments/Equipment.h"
#include "Util/random.h"
#include "Util/util.h"

Equipment* equipment_new(EquipmentType type) {
    Equipment* equipment = (Equipment*)malloc(sizeof(Equipment));
    if (equipment == NULL) {
        fprintf(stderr, "Equipment malloc error\n");
        exit(1);
    }

    equipment->type = type;

    return equipment;
}

void equipment_free(Equipment* equipment) {
    free(equipment);
}

Equipment* get_random_equipment(int difficulty) {
    int random = randrange(1, 3);  /* WEAPON, ARMOR, MAGICWAND */
    int tmp = randrange(difficulty * 0.5, difficulty * 2);
    int quality = MAX(1, tmp);


    Equipment* equipment = NULL;

    switch (random)
    {
    case 1:
        /* WEAPON */
        equipment = equipment_new(WEAPON);
        equipment->weapon.quality = quality;
        break;
    case 2:
        /* ARMOR */
        equipment = equipment_new(ARMOR);
        equipment->armor.quality = quality;
        break;
    case 3:
        /* MAGICWAND */
        equipment = equipment_new(MAGICWAND);
        equipment->magic_wand.quality = quality;
        break;
    default:
        fprintf(stderr, "Invalid Equipment type index\n");
        exit(1);
        break;
    }

    return equipment;
}