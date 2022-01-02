#include <stdlib.h>
#include <stdio.h>

#include "Entity/Inventory/Inventory.h"
#include "Util/random.h"


void init_inventory(Inventory* inventory) {
    int i;
    for (i = 0; i < NB_ITEMS; i++) {
        inventory->items[i] = NULL;
    }
}

Item* item_new(ItemType type) {
    Item* item = (Item*)malloc(sizeof(Item));
    if (item == NULL) {
        fprintf(stderr, "Item malloc error\n");
        exit(1);
    }

    item->type = type;

    return item;
}

void item_free(Item* item) {
    if (item->type == POTION) {
        potion_free(item->potion);
    }
    else if (item->type == EQUIPMENT) {
        equipment_free(item->equipment);
    }
    free(item);
}

Item* get_random_item(int difficulty) {
    int random = randrange(1, 2);  /* EQUIPMENT, POTION */

    ItemType type = (random == 1) ? EQUIPMENT : POTION;
    Item* item = item_new(type);

    if (type == POTION) {
        item->potion = get_random_potion();
    } else if (type == EQUIPMENT) {
        item->equipment = get_random_equipment(difficulty);
    }

    return item;
}