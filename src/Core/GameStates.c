#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "Core/GameStates.h"
#include "Map/Map.h"
#include "Util/ArrayList.h"
#include "Util/Element.h"
#include "Util/PathFinding/Astar.h"
#include "constants.h"

static bool is_walkable_func(Element* grid, int i, int j) {
    Map* map = (Map*)grid;
    Cell* cell = get_cell(map, i, j);
    return cell->type != WALL;
}

void init_game_states(GameStates* gs) {
    gs->maps = arrayList_new();
    gs->current_stage = 0;
    gs->path_to_stair = NULL;
    gs->inventory_is_open = false;

    Map* map = map_new();
    arrayList_add(gs->maps, map);

    init_player(get_player(gs), START_X, START_Y);

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
                player->pos.x = i;
                player->pos.y = j;

                return;
            }
        }
    }
}

static void update_stair(Map* map) {
    int i, j;

    for (j = 0; j < HEIGHT; j++) {
        for (i = 0; i < WIDTH; i++) {
            Cell* cell = get_cell(map, i, j);

            if (cell->type == STAIR_DOWN) {
                map->stair_down = cell;
                return;
            }
        }
    }
}

void go_next_stage(GameStates* gs) {
    gs->current_stage++;

    if (gs->current_stage > gs->maps->length - 1) {
        Map* map = map_new();
        arrayList_add(gs->maps, map);
    }

    set_player_position(gs, STAIR_UP);
    
    #if DEBUG
        update_stair(get_current_map(gs));
        update_path_to_stair(gs);
    #endif
}

void go_previous_stage(GameStates* gs) {
    if (gs->current_stage == 0) return;

    gs->current_stage--;

    set_player_position(gs, STAIR_DOWN);

    #if DEBUG
        update_stair(get_current_map(gs));
        update_path_to_stair(gs);
    #endif
}

void update_path_to_stair(GameStates* gs) {
    if (gs->path_to_stair != NULL) linkedList_free(gs->path_to_stair, free_path_position_element);

    Player* player = get_player(gs);
    Map* map = get_current_map(gs);

    PathPosition* start = path_position_new(player->pos.x, player->pos.y);
    PathPosition* end = path_position_new(map->stair_down->x, map->stair_down->y);

    gs->path_to_stair = astar(map, start, end, is_walkable_func, WIDTH, HEIGHT);
}