#include <assert.h>
#include "Core/GameStates.h"
#include "Map/Map.h"
#include "Util/ArrayList.h"
#include "Util/Element.h"
#include "constants.h"

void init_game_states(GameStates* gs) {
    gs->maps = arrayList_new();
    gs->current_stage = 0;

    Map* map = map_new();
    arrayList_add(gs->maps, map);

    init_player(get_player(gs), START_X, START_Y);
}

static void free_map(Element element) {
    map_free((Map*)element);
}

void destroy_game_states(GameStates* gs) {
    arrayList_free(gs->maps, free_map);
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

void go_next_stage(GameStates* gs) {
    gs->current_stage++;

    if (gs->current_stage > gs->maps->length - 1) {
        Map* map = map_new();
        arrayList_add(gs->maps, map);
    }

    set_player_position(gs, STAIR_UP);
}

void go_previous_stage(GameStates* gs) {
    if (gs->current_stage == 0) return;

    gs->current_stage--;

    set_player_position(gs, STAIR_DOWN);
}