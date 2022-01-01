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

typedef struct Potion{
    PotionType type;
    union {
        Accuracy accuracy;
        Experience experience;
        Health health;
        Magic magic;
        Regeneration regeneration;
    };
} Potion;


/**
 * @brief Crée une nouvelle potion
 * 
 * @param type 
 * @return Potion* 
 */
Potion* potion_new(PotionType type);

/**
 * @brief Libère la mémoire allouée par la potion
 * 
 * @param potion 
 */
void potion_free(Potion* potion);

/**
 * @brief Renvoie une potion aléatoire
 *  
 * @return Potion* 
 */
Potion* get_random_potion();


#endif