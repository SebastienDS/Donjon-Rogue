#include "Entity/Treasure.h"
#include <stdlib.h>

void init_treasure(Treasure* treasure, int difficulty){
    int i;

    treasure->state = CLOSE;
    for (i = 0; i < ITEMS_PER_TREASURE; i++){
        treasure->items[i] = get_random_item(difficulty);
    }
}

void destroy_treasure(Treasure* treasure){
    int i;

    for (i = 0; i < ITEMS_PER_TREASURE; i++){
        if (treasure->items[i] == NULL) continue;

        item_free(treasure->items[i]);
        treasure->items[i] = NULL;
    }
}