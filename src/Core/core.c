#include "Core/core.h"
#include "Core/GameStates.h"
#include "Core/Events.h"
#include "Core/Action.h"
#include "Entity/Player.h"
#include "Map/Map.h"
#include "Interface/graphics.h"
#include "Interface/interface.h"
#include "Interface/Images.h"


static bool update_player_movement(GameStates* gs, Events* events) {
    if (!(events->event == MLV_KEY && events->state == MLV_PRESSED)) return false;

    Player* player = &gs->player;
    Map* map = &gs->map;
    
    int dx = 0, dy = 0;

    switch (events->key) {
        case MLV_KEYBOARD_z:
        case MLV_KEYBOARD_UP:
            dy = -1;
            break;
        case MLV_KEYBOARD_s:
        case MLV_KEYBOARD_DOWN:
            dy = 1;
            break;
        case MLV_KEYBOARD_q:
        case MLV_KEYBOARD_LEFT:
            dx = -1;
            break;
        case MLV_KEYBOARD_d:
        case MLV_KEYBOARD_RIGHT:
            dx = 1;
            break;
        default:
            break;
    }

    return try_move(map, player, dx, dy);
}

static void update_player_action(GameStates* gs, Events* events, Action* action) {
    if (!(events->event == MLV_KEY && events->state == MLV_PRESSED)) return;

    Player* player = &gs->player;
    Map* map = &gs->map;

    Cell* cell = NULL;
    
    switch (events->key) {
        case MLV_KEYBOARD_z:
        case MLV_KEYBOARD_UP:
            cell = get_cell(map, player->pos.x, player->pos.y - 1);
            break;
        case MLV_KEYBOARD_s:
        case MLV_KEYBOARD_DOWN:
            cell = get_cell(map, player->pos.x, player->pos.y + 1);
            break;
        case MLV_KEYBOARD_q:
        case MLV_KEYBOARD_LEFT:
            cell = get_cell(map, player->pos.x - 1, player->pos.y);
            break;
        case MLV_KEYBOARD_d:
        case MLV_KEYBOARD_RIGHT:
            cell = get_cell(map, player->pos.x + 1, player->pos.y);
            break;
        default:
            break;
    }


    if (cell == NULL) return;

    if (cell->type == TREASURE) {
        action->type = OPEN_TREASURE;
        action->cell = cell;
        fprintf(stderr, "TREASURE");
    }
    else if (cell->type == MONSTER) {
        action->type = FIGHT_MONSTER;
        action->cell = cell;
        fprintf(stderr, "MONSTER");
    }
}

void init(GameStates* gs) {
    init_map(&gs->map);
    generate_stage(&gs->map);

    init_player(&gs->player, START_X, START_Y);
}

void update(GameStates* gs, Events* events) {
    bool player_updated = update_player_movement(gs, events);

    if (player_updated){
        return;
    } 
    Action* action = action_new();
    update_player_action(gs, events, action);
    apply_action(action, gs);
    action_free(action);
}

void draw(GameStates* gs, Images* images) {
    MLV_clear_window(MLV_COLOR_BLACK);
    
    draw_graphics(gs, images);
    draw_interface(gs);
}