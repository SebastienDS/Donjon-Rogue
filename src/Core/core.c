#include <MLV/MLV_all.h>

#include "Core/core.h"
#include "Core/GameStates.h"
#include "Core/Events.h"
#include "Core/Action.h"
#include "Entity/Player.h"
#include "Map/Map.h"
#include "Interface/graphics.h"
#include "Interface/raycastingGraphics.h"
#include "Interface/interface.h"
#include "Interface/View.h"
#include "constants.h"


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

    bool moved = try_move(player, map, dx, dy);

    #if DEBUG
        if (moved) update_path_to_stair(gs);
    #endif
    return moved;
}

static bool update_player_movement_3d(GameStates* gs, Events* events) {
    if (gs->viewType != RAYCASTING) return false;
    if (!(events->event == MLV_KEY && events->state == MLV_PRESSED)) return false;

    Player* player = get_player(gs);
    Map* map = get_current_map(gs);

    int dx = 0, dy = 0;
    
    switch (events->key) {
        case MLV_KEYBOARD_z:
        case MLV_KEYBOARD_UP:
            dx = player->direction.x;
            dy = player->direction.y;
            break;
        case MLV_KEYBOARD_s:
        case MLV_KEYBOARD_DOWN:
            dx = -player->direction.x;
            dy = -player->direction.y;
            break;
        case MLV_KEYBOARD_q:
        case MLV_KEYBOARD_LEFT:
            rotate_player(player, -M_PI / 4.0);
            return true;
        case MLV_KEYBOARD_d:
        case MLV_KEYBOARD_RIGHT:
            rotate_player(player, M_PI / 4.0);
            return true;
        default:
            break;
    }

    return try_move(player, map, dx, dy);
}

static bool update_action_from_player_movement(GameStates* gs, Events* events, Action* action) {
    if (!(events->event == MLV_KEY && events->state == MLV_PRESSED)) return false;

    Player* player = get_player(gs);
    Map* map = get_current_map(gs);

    Cell* cell = NULL;
    
    switch (events->key) {
        case MLV_KEYBOARD_z:
        case MLV_KEYBOARD_UP:
            cell = get_cell(map, player->position.x, player->position.y - 1);
            break;
        case MLV_KEYBOARD_s:
        case MLV_KEYBOARD_DOWN:
            cell = get_cell(map, player->position.x, player->position.y + 1);
            break;
        case MLV_KEYBOARD_q:
        case MLV_KEYBOARD_LEFT:
            cell = get_cell(map, player->position.x - 1, player->position.y);
            break;
        case MLV_KEYBOARD_d:
        case MLV_KEYBOARD_RIGHT:
            cell = get_cell(map, player->position.x + 1, player->position.y);
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

static bool test_click(Button* button, int x, int y){
    return button->x <= x && button->x + button->width >= x 
        && button->y <= y && button->y + button->height >= y;
}

static bool update_action_from_mouse(GameStates* gs, Events* events, Action* action) {
    if (!(events->event == MLV_MOUSE_BUTTON && events->state == MLV_PRESSED)) return false;

    if (gs->inventory.is_open) {
        if(test_click(&gs->inventory.equip, events->mouseX, events->mouseY)) {
            gs->inventory.equip.callback(gs);  
            return true; 
        }
        else if(test_click(&gs->inventory.use, events->mouseX, events->mouseY)) {
            gs->inventory.use.callback(gs);   
            return true; 
        }
        else if(test_click(&gs->inventory.throw, events->mouseX, events->mouseY)) {
            gs->inventory.throw.callback(gs);   
            return true; 
        }
    }
}

static bool update_action_from_input(GameStates* gs, Events* events, Action* action) {
    if (!(events->event == MLV_KEY && events->state == MLV_PRESSED)) return false;

    Player* player = get_player(gs);
    Map* map = get_current_map(gs);

    Cell* cell;

    switch (events->key) {
        case MLV_KEYBOARD_e:
            cell = get_cell(map, player->position.x, player->position.y);
            action->type = USE_STAIR;
            action->cell = cell;
            return true;
        case MLV_KEYBOARD_i:
            gs->inventory.is_open = !gs->inventory.is_open;
            return true;
        case MLV_KEYBOARD_c:
            gs->viewType = gs->viewType == DEFAULT ? RAYCASTING : DEFAULT;
            return true;
        default:
            break;
    }

    return false;
}

void update(GameStates* gs, Events* events) {
    Action* action = action_new();

    update_action_from_input(gs, events, action)
        || gs->viewType == RAYCASTING 
            ? update_player_movement_3d(gs, events)
            : update_action_from_player_movement(gs, events, action) || update_player_movement(gs, events)
        || update_action_from_mouse(gs, events, action);
        
    apply_action(action, gs);
    action_free(action);
}

void draw(GameStates* gs, View* view) {
    MLV_clear_window(MLV_COLOR_BLACK);
    
    if (gs->viewType == DEFAULT) {
        draw_graphics(gs, view);
    }
    else if (gs->viewType == RAYCASTING) {
        draw_raycasting_graphics(gs, view);
    }
    else {
        fprintf(stderr, "Unknown view type: %d\n", gs->viewType);
        exit(1);
    }

    if (gs->viewType == RAYCASTING) {
        static int cell_size = 25;
        int w = VISION_X * cell_size;
        draw_mini_map(gs, view, SCREEN_WIDTH - w - 15, 75, cell_size);
    }

    draw_interface(gs, view);
}

