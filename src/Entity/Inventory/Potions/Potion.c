#include <stdlib.h>
#include <stdio.h>

#include "Entity/Inventory/Potions/Potion.h"
#include "Util/random.h"


Potion* potion_new(PotionType type) {
    Potion* potion = (Potion*)malloc(sizeof(Potion));
    if (potion == NULL) {
        fprintf(stderr, "Potion malloc error\n");
        exit(1);
    }

    potion->type = type;

    return potion;
}

void potion_free(Potion* potion) {
    free(potion);
}

Potion* get_random_potion() {
    int random = randrange(1, 5);  /* ACCURACY, EXPERIENCE, HEALTH, MAGIC, REGENERATION */

    Potion* potion = NULL;

    switch (random)
    {
    case 1:
        /* HEALTH */
        potion = potion_new(HEALTH);
        potion->health.hp = 10;
        break;
    case 2:
        /* MAGIC */
        potion = potion_new(MAGIC);
        potion->magic.mp = 10;
        break;
    case 3:
        /* REGENERATION */
        potion = potion_new(REGENERATION);
        potion->regeneration.hp = 20;
        potion->regeneration.mp = 10;
        potion->regeneration.interval = 3;
        potion->regeneration.countdown = 0;
        potion->regeneration.duration = 30;
        break;
    case 4:
        /* ACCURACY */
        potion = potion_new(ACCURACY);
        potion->accuracy.crit = 15;
        potion->accuracy.duration = 30;
        break;
    case 5:
        /* EXPERIENCE */
        potion = potion_new(EXPERIENCE);
        potion->experience.exp = 30;
        potion->experience.duration = 30;
        break;
    
    default:
        fprintf(stderr, "Invalid Potion type index\n");
        exit(1);
        break;
    }

    return potion;
}

