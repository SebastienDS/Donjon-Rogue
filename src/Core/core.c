#include <MLV/MLV_all.h>

#include "Core/core.h"
#include "Core/GameStates.h"
#include "Core/Events.h"
#include "Core/Action.h"
#include "Entity/Player.h"
#include "Entity/Monster.h"
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

    if (moved) {
        #if DEBUG
            update_path_to_stair(gs);
        #endif
        
        gs->end_turn = true;
    }
    
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

    bool moved = try_move(player, map, dx, dy);

    if (moved) {
        gs->end_turn = true;
    }
    return moved;
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
        fprintf(stderr, "TREASURE\n");
        return true;
    }
    else if (cell->type == MONSTER) {
        action->type = FIGHT_MONSTER;
        action->cell = cell;
        fprintf(stderr, "MONSTER\n");
        return true;
    }
    return false;
}

static bool test_click(Button* button, int x, int y){
    return button->x <= x && button->x + button->width >= x 
        && button->y <= y && button->y + button->height >= y;
}

static bool verif_click_item_inventory(int x, int y){
    static int start_x = SCREEN_WIDTH * 3 / 5 + 15;
    static int start_y = 30;
    static int width_items = (SCREEN_WIDTH * 2 / 5 - 45) / 4;

    int width = width_items * 4;
    int height = width_items * 3;

    return start_x <= x && start_x + width >= x
        && start_y <= y && start_y + height >= y;
}

static void get_item_selected(GameStates* gs, int x, int y){
    static int start_x = SCREEN_WIDTH * 3 / 5 + 15;
    static int start_y = 30;
    static int width_items = (SCREEN_WIDTH * 2 / 5 - 45) / 4;

    int i = (x - start_x) / width_items;
    int j = (y - start_y) / width_items;
    
    gs->inventory.item_selected = get_player(gs)->inventory.items[i + j * 4];
    gs->inventory.index = i + j * 4;
}

static bool update_action_from_mouse(GameStates* gs, Events* events, Action* action) {
    if (!(events->event == MLV_MOUSE_BUTTON && events->state == MLV_PRESSED)) return false;
    
    Item* item = gs->inventory.item_selected;
    Player* player = get_player(gs);

    if (gs->inventory.is_open) {
        if(verif_click_item_inventory(events->mouseX, events->mouseY)) get_item_selected(gs, events->mouseX, events->mouseY);

        if (item == NULL) return false;

       if(item->type == POTION && test_click(&gs->inventory.use, events->mouseX, events->mouseY)) {
            gs->inventory.use.callback(gs);   
            return true; 
        }
        else if(!verif_equiped(player, item) && test_click(&gs->inventory.throw, events->mouseX, events->mouseY)) {
            gs->inventory.throw.callback(gs);   
            return true; 
        }
    }
    return false;
}

static bool update_action_from_input(GameStates* gs, Events* events, Action* action) {
    if (!(events->event == MLV_KEY && events->state == MLV_PRESSED)) return false;

    Player* player = get_player(gs);
    Map* map = get_current_map(gs);

    Cell* player_cell;
    Cell* cell;
    ArrayList* cells = NULL;

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
        case MLV_KEYBOARD_t:
            player_cell = get_cell(map, player->position.x, player->position.y);
            cells = find_neighbors(map, player_cell, TREASURE);    
            if (cells->length == 0) break;

            action->type = OPEN_TREASURE;
            action->cell = arrayList_get(cells, 0);

            arrayList_free(cells, NULL);
            return true;
        case MLV_KEYBOARD_a:
            player_cell = get_cell(map, player->position.x, player->position.y);
            cells = find_neighbors(map, player_cell, MONSTER);
            if (cells->length == 0) break;

            action->type = FIGHT_MONSTER;
            action->cell = arrayList_get(cells, 0);

            arrayList_free(cells, NULL);
            return true;
        case MLV_KEYBOARD_p:
            player->attackType = PHYSICAL;
            return true;
        case MLV_KEYBOARD_m:
            player->attackType = MAGICAL;
            return true;
        default:
            break;
    }

    if (cells != NULL) arrayList_free(cells, NULL);
    return false;
}

static void end_turn(GameStates* gs) {
    Map* map = get_current_map(gs);
    Player *p = get_player(gs);
    Cell* cell = get_cell(map, p->position.x, p->position.y);

    ArrayList* monsters = find_neighbors(map, cell, MONSTER);

    int i;
    for (i = 0; i < monsters->length; i++) {
        Monster* monster = &((Cell*)arrayList_get(monsters, i))->monster;
        attack_player(monster, p);
    }

    arrayList_free(monsters, NULL);
    update_bonus(p);
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

    if (gs->end_turn) {
        end_turn(gs);
        gs->end_turn = false;
    }
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

