#include "Core/Action.h"
#include "Core/GameStates.h"
#include "Entity/Monster.h"
#include "Map/Map.h"

#include <stdio.h>
#include <stdlib.h>

Action* action_new(void){
    Action* action = (Action*) malloc (sizeof(Action));

    if (action == NULL){
        fprintf(stderr, "action malloc error");
        exit(1);
    }

    action->type = NONE;
    return action;
}

static void remove_if_dead(Cell* cell){
    if (is_dead(&cell->monster)){
        cell->type = ROOM;
    }
}

void apply_action(Action* action, GameStates* gs) {
    Cell* cell;

    switch (action->type) {
        case OPEN_TREASURE:
            action->cell->treasure.state = OPEN;
            break;
        
        case FIGHT_MONSTER:
            physical_attack(get_player(gs), &action->cell->monster);
            printf("hp : %d \n", action->cell->monster.hp);
            remove_if_dead(action->cell);
            break;
        case USE_STAIR:
            cell = action->cell;
            
            if (cell->type == STAIR_DOWN) {
                go_next_stage(gs);
            } else if (cell->type == STAIR_UP) {
                go_previous_stage(gs);
            }
            break;
        default:
            break;
    }
}

void action_free(Action* action){
    free(action);
}
