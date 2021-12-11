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

    Player* player = get_player(gs);
    Map* map = get_current_map(gs);
    
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

static bool update_action_from_player_movement(GameStates* gs, Events* events, Action* action) {
    if (!(events->event == MLV_KEY && events->state == MLV_PRESSED)) return false;

    Player* player = get_player(gs);
    Map* map = get_current_map(gs);

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


    if (cell == NULL) return false;

    if (cell->type == TREASURE && cell->treasure.state == CLOSE) {
        action->type = OPEN_TREASURE;
        action->cell = cell;
        fprintf(stderr, "TREASURE");
        return true;
    }
    else if (cell->type == MONSTER) {
        action->type = FIGHT_MONSTER;
        action->cell = cell;
        fprintf(stderr, "MONSTER");
        return true;
    }
    return false;
}

static bool update_action_from_input(GameStates* gs, Events* events, Action* action) {
    if (!(events->event == MLV_KEY && events->state == MLV_PRESSED)) return false;

    Player* player = get_player(gs);
    Map* map = get_current_map(gs);

    Cell* cell;

    switch (events->key) {
        case MLV_KEYBOARD_e:
            cell = get_cell(map, player->pos.x, player->pos.y);
            action->type = USE_STAIR;
            action->cell = cell;
            return true;
        default:
            break;
    }

    return false;
}

void update(GameStates* gs, Events* events) {
    Action* action = action_new();

    update_action_from_input(gs, events, action)
        || update_action_from_player_movement(gs, events, action)
        || update_player_movement(gs, events);

    apply_action(action, gs);
    action_free(action);
}

void draw(GameStates* gs, Images* images) {
    MLV_clear_window(MLV_COLOR_BLACK);
    
    draw_graphics(gs, images);
    draw_interface(gs);
}