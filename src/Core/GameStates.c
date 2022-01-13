#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Core/GameStates.h"
#include "Entity/Player.h"
#include "Map/Map.h"
#include "Util/ArrayList.h"
#include "Util/Element.h"
#include "Util/PathFinding/Astar.h"
#include "constants.h"

#include "Entity/Inventory/Potions/potionEffect.h"

static void use_callback(GameStates* gs){
    Player* player = get_player(gs);
    bool used = use_potion(gs->inventory.item_selected->potion, player);

    if (used){
        gs->inventory.item_selected->potion = NULL;
        item_free(gs->inventory.item_selected);
        gs->inventory.item_selected = NULL;
        player->inventory.items[gs->inventory.index] = NULL;
    }
}

static void throw_callback(GameStates* gs){
    Player* player = get_player(gs);

    if(verif_equiped(player, gs->inventory.item_selected)) return;
    
    item_free(gs->inventory.item_selected);
    gs->inventory.item_selected = NULL;
    player->inventory.items[gs->inventory.index] = NULL;
}

static void take_callback(GameStates* gs) {
    Player* player = get_player(gs);
    int index = get_empty_slot(player);

    if (index == -1) return;

    insert_item(player, gs->treasure.item_selected, index);
    gs->treasure.item_selected = NULL;
    gs->treasure.treasure->items[gs->treasure.index] = NULL;
}

static void close_treasure_callback(GameStates* gs) {
    destroy_treasure(gs->treasure.treasure);
    gs->treasure.treasure = NULL;
    gs->treasure.item_selected = NULL;
    gs->treasure.is_open = false;
    gs->inventory.is_open = false;

    gs->treasure.cell->type = ROOM;
}

static void upgrade_atk_callback(GameStates* gs) {
    Player* player = get_player(gs);
    player->atk++;
    player->skill_points--;
}

static void upgrade_intel_callback(GameStates* gs) {
    Player* player = get_player(gs);
    player->intel++;
    player->skill_points--;
}

static void upgrade_def_callback(GameStates* gs) {
    Player* player = get_player(gs);
    player->def++;
    player->skill_points--;
}

static bool is_walkable_func(Element* grid, int i, int j) {
    Map* map = (Map*)grid;
    Cell* cell = get_cell(map, i, j);
    return cell->type != WALL;
}

void set_button(Button* button, int x, int y, int width, int height, char label[50], void(*callback)(GameStates* gs)) {
    button->x = x;
    button->y = y;
    button->width = width;
    button->height = height;
    strcpy(button->label, label);
    button->callback = callback;
}

void init_game_states(GameStates* gs) {
    gs->viewType = DEFAULT;

    gs->end_turn = false;

    gs->maps = arrayList_new();
    gs->current_stage = 0;
    gs->path_to_stair = NULL;
    
    gs->inventory.is_open = false;
    gs->inventory.item_selected = NULL;
    gs->inventory.index = 0;

    gs->treasure.is_open = false;
    gs->treasure.item_selected = NULL;
    gs->treasure.index = 0;
    gs->treasure.treasure = NULL;
    gs->treasure.cell = NULL;


    set_button(&gs->inventory.use, SCREEN_WIDTH * 3 / 5 + 15, SCREEN_HEIGHT - 45 - 50, (SCREEN_WIDTH * 2 / 5 - 15) / 2 - 30, 50, "USE", use_callback); 
    set_button(&gs->inventory.throw, SCREEN_WIDTH - ((SCREEN_WIDTH * 2 / 5 - 15) / 2 - 30) - 30, SCREEN_HEIGHT - 45 - 50, (SCREEN_WIDTH * 2 / 5 - 15) / 2 - 30, 50, "THROW", throw_callback);

    set_button(&gs->treasure.take, (SCREEN_WIDTH * 2 / 5 - 15 - ((SCREEN_WIDTH * 2 / 5 - 15) / 2 - 30)) / 2, SCREEN_HEIGHT - 45 - 50, (SCREEN_WIDTH * 2 / 5 - 15) / 2 - 30, 50, "TAKE", take_callback);
    set_button(&gs->treasure.close, SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT * 0.8, 150, 75, "CLOSE", close_treasure_callback);

    set_button(&gs->skills_btn.atk, 150, 185, 25, 25, "+", upgrade_atk_callback);
    set_button(&gs->skills_btn.intel, 150, 215, 25, 25, "+", upgrade_intel_callback);
    set_button(&gs->skills_btn.def, 150, 245, 25, 25, "+", upgrade_def_callback);

    Player* p = get_player(gs);
    init_player(p, START_X, START_Y);

    Map* map = map_new();
    generate_stage(map, gs->current_stage + 1, p->lvl);
    arrayList_add(gs->maps, map);

    int i;
    for (i = 0; i < LOG_COUNT; i++) {
        gs->logs[i] = NULL;
    }

    #if DEBUG
        update_path_to_stair(gs);
    #endif
}

static void free_map(Element element) {
    map_free((Map*)element);
}

void destroy_game_states(GameStates* gs) {
    arrayList_free(gs->maps, free_map);
    linkedList_free(gs->path_to_stair, free_path_position_element);
}

Player* get_player(GameStates* gs) {
    return &gs->player;
}

Map* get_current_map(GameStates* gs) {
    return arrayList_get(gs->maps, gs->current_stage);
}

static void set_player_position(GameStates* gs, Celltype cell_type) {
    Map* map = get_current_map(gs);
    Player* player = get_player(gs);

    int i, j;

    for (j = 0; j < HEIGHT; j++) {
        for (i = 0; i < WIDTH; i++) {
            Cell* cell = get_cell(map, i, j);

            if (cell->type == cell_type) {
                player->position.x = i;
                player->position.y = j;

                return;
            }
        }
    }
}

void go_next_stage(GameStates* gs) {
    gs->current_stage++;

    if (gs->current_stage > gs->maps->length - 1) {
        Player* p = get_player(gs);
        Map* map = map_new();
        generate_stage(map, gs->current_stage + 1, p->lvl);
        arrayList_add(gs->maps, map);
    }

    set_player_position(gs, STAIR_UP);
    
    #if DEBUG
        update_stair_down(get_current_map(gs));
        update_path_to_stair(gs);
    #endif
}

void go_previous_stage(GameStates* gs) {
    if (gs->current_stage == 0) return;

    gs->current_stage--;

    set_player_position(gs, STAIR_DOWN);

    #if DEBUG
        update_stair_down(get_current_map(gs));
        update_path_to_stair(gs);
    #endif
}

void update_path_to_stair(GameStates* gs) {
    if (gs->path_to_stair != NULL) linkedList_free(gs->path_to_stair, free_path_position_element);

    Player* player = get_player(gs);
    Map* map = get_current_map(gs);

    PathPosition* start = path_position_new(player->position.x, player->position.y);
    PathPosition* end = path_position_new(map->stair_down->x, map->stair_down->y);

    gs->path_to_stair = astar(map, start, end, is_walkable_func, WIDTH, HEIGHT);
}

void swap_str(char** str1, char** str2) {
    char* temp = *str1;
    *str1 = *str2;
    *str2 = temp;
}  

void log_event(GameStates* gs, char* log) {
    swap_str(&log, &gs->logs[0]);

    int i;
    for (i = 0; i < LOG_COUNT - 1; i++) {
        swap_str(&gs->logs[i], &gs->logs[i + 1]);
    }
}