#include "Core/Action.h"
#include "Core/GameStates.h"

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

void apply_action(Action* action, GameStates* gs){
    switch (action->type) {
        case OPEN_TREASURE:
            action->cell->treasure.state = OPEN;
            break;
        
        default:
            break;
    }
}

void action_free(Action* action){
    free(action);
}
