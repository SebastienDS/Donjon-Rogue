#ifndef __POTION__
#define __POTION__

#include "Entity/Inventory/Potions/Accuracy.h"
#include "Entity/Inventory/Potions/Experience.h"
#include "Entity/Inventory/Potions/Health.h"
#include "Entity/Inventory/Potions/Magic.h"
#include "Entity/Inventory/Potions/Regeneration.h"

typedef enum {
    ACCURACY, EXPERIENCE, HEALTH, MAGIC, REGENERATION
} PotionType;

typedef struct {
    PotionType type;
    union {
        Accuracy accuracy;
        Experience experience;
        Health health;
        Magic magic;
        Regeneration regeration;
    };
} Potion;

#endif