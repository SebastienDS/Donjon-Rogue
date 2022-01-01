#ifndef __POTIONEFFECT__
#define __POTIONEFFECT__

#include "Entity/Inventory/Potions/Potion.h"
#include "Entity/Player.h"

#include <stdbool.h>


/**
 * @brief Affecte la potion accuracy au joueur
 * 
 * @param potion 
 * @param player 
 * 
 * @return bool
 */
bool use_potion_accuracy(Potion* potion, Player* player);

/**
 * @brief Affecte la potion health au joueur
 * 
 * @param potion 
 * @param player 
 * 
 * @return bool
 */
bool use_potion_health(Potion* potion, Player* player);

/**
 * @brief Affecte la potion magic au joueur
 * 
 * @param potion 
 * @param player 
 * 
 * @return bool
 */
bool use_potion_magic(Potion* potion, Player* player);


/**
 * @brief Affecte l'effet de la potion regeneration au joueur
 * 
 * @param potion 
 * @param player 
 * 
 * @return bool
 */
bool use_potion_regeneration(Potion* potion, Player* player);

/**
 * @brief Affetce l'effet de la potion experience au joueur
 * 
 * @param potion 
 * @param player 
 * 
 * @return bool
 */
bool use_potion_experience(Potion* potion, Player* player);

/**
 * @brief Affecte l'effet de la potion au joueur
 * 
 * @param potion 
 * @param player
 * 
 * @return bool
 */
bool use_potion(Potion* potion, Player* player);

/**
 * @brief Applique la potion regeneration
 * 
 * @param potion 
 * @param player 
 * @return true 
 * @return false 
 */
bool apply_potion_regeneration(Potion* potion, Player* player);

/**
 * @brief Applique la potion accuracy
 * 
 * @param potion 
 * @param player 
 * @return true 
 * @return false 
 */
bool apply_potion_accuracy(Potion* potion, Player* player);

/**
 * @brief Applique la potion experience
 * 
 * @param potion 
 * @param player 
 * @return true 
 * @return false 
 */
bool apply_potion_experience(Potion* potion, Player* player);

#endif