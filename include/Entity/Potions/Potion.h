#ifndef __POTION__
#define __POTION__

#include "Entity/Potions/Accuracy.h"
#include "Entity/Potions/Experience.h"
#include "Entity/Potions/Health.h"
#include "Entity/Potions/Magic.h"
#include "Entity/Potions/Regeneration.h"

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