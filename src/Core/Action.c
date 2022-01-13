#include "Core/Action.h"
#include "Core/GameStates.h"
#include "Entity/Monster.h"
#include "Entity/Player.h"
#include "Map/Map.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

Action* action_new(void){
    Action* action = (Action*) malloc (sizeof(Action));

    if (action == NULL){
        fprintf(stderr, "action malloc error");
        exit(1);
    }

    action->type = NONE;
    return action;
}

static void remove_if_dead(Cell* cell, Player* player){
    if (is_monster_dead(&cell->monster)){
        gain_experience(player, cell->monster.exp_given);
        cell->type = ROOM;
    }
}

void apply_action(Action* action, GameStates* gs) {
    Cell* cell;
    bool attack_succeed;

    switch (action->type) {
        case OPEN_TREASURE:
            gs->treasure.treasure = &action->cell->treasure;
            action->cell->treasure.state = OPEN;
            gs->treasure.is_open = true;
            gs->treasure.cell = action->cell;
            log_event(gs, "Open Treasure");

        case OPEN_INVENTORY:
            gs->inventory.is_open = true;
            break;

        case CLOSE_TREASURE:
            destroy_treasure(gs->treasure.treasure);
            gs->treasure.treasure = NULL;
            gs->treasure.item_selected = NULL;
            gs->treasure.is_open = false;
            action->cell->type = ROOM;

        case CLOSE_INVENTORY:
            gs->inventory.is_open = false;
            break;

        case TRIGGER_INVENTORY:
            gs->inventory.is_open = !gs->inventory.is_open;
            break;

        case FIGHT_MONSTER:
            attack_succeed = attack_monster(get_player(gs), &action->cell->monster, action->attackType);
            if (attack_succeed) {
                remove_if_dead(action->cell, get_player(gs));
                log_event(gs, "Fight Monster");
                
                gs->end_turn = true;
            } else {
                log_event(gs, "Attack Failed: Not enought mana");
            }
            break;

        case USE_STAIR:
            cell = action->cell;
            
            if (cell->type == STAIR_DOWN) {
                go_next_stage(gs);
            } else if (cell->type == STAIR_UP) {
                go_previous_stage(gs);
            }

            log_event(gs, "Use Stair");
            break;

        default:
            break;
    }
}

void action_free(Action* action){
    free(action);
}


