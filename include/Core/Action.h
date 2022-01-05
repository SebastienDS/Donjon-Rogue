#ifndef __ACTION__
#define __ACTION__

#include "Map/Map.h"
#include "Core/GameStates.h"

typedef enum {
    NONE, 
    OPEN_TREASURE, 
    CLOSE_TREASURE, 
    FIGHT_MONSTER, 
    OPEN_INVENTORY, 
    CLOSE_INVENTORY, 
    TRIGGER_INVENTORY, 
    USE_STAIR
} ActionType;

typedef struct {
    ActionType type;
    Cell* cell;
    AttackType attackType;
} Action;

/**
 * @brief Crée une action et la renvoie
 * 
 * @return Action* 
 */
Action* action_new(void);

/**
 * @brief Libère la mémoire allouée pour l'action
 * 
 * @param action 
 */
void action_free(Action* action);

/**
 * @brief Applique l'action
 * 
 * @param action 
 * @param gs 
 */
void apply_action(Action* action, GameStates* gs);



#endif