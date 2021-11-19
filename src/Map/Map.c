#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "Map/Map.h"
#include "Util/ArrayList.h"
#include "Util/random.h"


void init_map(Map* self) {
    int i, j;

    for (j = 0; j < HEIGHT; j++) {
        for (i = 0; i < WIDTH; i++) {
            self->map[j][i].type = WALL;
            self->map[j][i].x = i;
            self->map[j][i].y = j;
        }
    }
    self->map[HEIGHT / 2][WIDTH / 2].type = ROOM;
}

static int compute_neighbors(Map* map, Cell* cell, int neighbors[][2], int length, Celltype type) {
    int i, pos_x, pos_y;
    int cpt = 0;
    
    for (i = 0; i < length; i++) {
        pos_x = neighbors[i][0] + cell->x;
        pos_y = neighbors[i][1] + cell->y;

        if (map->map[pos_y][pos_x].type == type) {
            cpt++;
        } 
    }

    return cpt;
}

static bool is_distance_1_eligible(Map* map, Cell* cell) {
    static const int neighbors_dist1[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    int cpt = compute_neighbors(map, cell, neighbors_dist1, 4, ROOM);
    return cpt == 1;
}

static bool is_distance_2_eligible(Map* map, Cell* cell) {
    static const int neighbors_dist2[8][2] = {{0, 2}, {0, -2}, {2, 0}, {-2, 0}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}};

    int cpt = compute_neighbors(map, cell, neighbors_dist2, 8, ROOM);
    return cpt <= 2;
}

bool is_eligible(Map* map, Cell* cell) {
    return map->map[cell->y][cell->x].type == WALL
        && is_distance_1_eligible(map, cell)
        && is_distance_2_eligible(map, cell);
}

static void init_adjacent(Map* map, ArrayList* list) {
    static const int neighbors_dist1[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    int i, pos_x, pos_y;
    int x_center = WIDTH / 2;
    int y_center = HEIGHT / 2;

    for (i = 0; i < 4; i++) {
        pos_x = neighbors_dist1[i][0] + x_center;
        pos_y = neighbors_dist1[i][1] + y_center;

        Cell* cell = &(map->map[pos_y][pos_x]);
        arrayList_add(list, cell);
    }
}

bool is_at_the_border(int x, int y) {
    return x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1;
}

static void fill_adjacent(Map* map, ArrayList* list, Cell* cell) {
    static const int neighbors_dist1[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    int i, pos_x, pos_y;

    for (i = 0; i < 4; i++) {
        pos_x = neighbors_dist1[i][0] + cell->x;
        pos_y = neighbors_dist1[i][1] + cell->y;
        Cell* new_cell = &(map->map[pos_y][pos_x]);

        if (is_eligible(map, new_cell) && !is_at_the_border(pos_x, pos_y) 
            && !arrayList_contains(list, new_cell)) {
            arrayList_add(list, new_cell);
        }
    }
}

static void expand_room(Map* map, ArrayList *toexpand) {
    Cell* cell;

    do {
        do {
            if (toexpand->length == 0) {
                return;
            }

            int index = randrange(0, toexpand->length - 1);
            cell = (Cell*) arrayList_remove(toexpand, index);
        } while (!is_eligible(map, cell));

        cell->type = ROOM;
        fill_adjacent(map, toexpand, cell);
    } while (toexpand->length != 0);
}


static bool should_be_wall(Map* map, Cell* cell) {
    static const int neighbors_dist1[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    int cpt = compute_neighbors(map, cell, neighbors_dist1, 4, WALL);
    return cpt >= 3;
}

static void get_room_to_be_wall(Map* map, ArrayList* room_to_wall) {
    int i, j;
    Cell* cell;

    for (j = 0; j < HEIGHT; j++) {
        for (i = 0; i < WIDTH; i++) {
            cell = &map->map[j][i];

            if (cell->type == ROOM && should_be_wall(map, cell)) {
                arrayList_add(room_to_wall, cell);
            }
        }
    }
}

static void transform_room_to_wall(ArrayList* room_to_wall) {
    int i;
    for (i = 0; i < room_to_wall->length; i++){
        Cell* cell = (Cell*) arrayList_get(room_to_wall, i);
        cell->type = WALL;
    }
}

static void fill_wall(Map* map) {
    ArrayList* room_to_wall = arrayList_new();
    
    get_room_to_be_wall(map, room_to_wall);
    transform_room_to_wall(room_to_wall);

    arrayList_free(room_to_wall, NULL);
} 

static Cell* select_closest_room(Map* map, Cell* cell){
    static const int neighbors_dist1[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    ArrayList* list_rooms = arrayList_new();
    int i, pos_x, pos_y;
    
    for (i = 0; i < 4; i++) {
        pos_x = neighbors_dist1[i][0] + cell->x;
        pos_y = neighbors_dist1[i][1] + cell->y;
        Cell* new_cell = &map->map[pos_y][pos_x];

        if (new_cell->type == ROOM) {
            arrayList_add(list_rooms, new_cell);
        } 
    }
    Cell* rand_cell = arrayList_get(list_rooms, randrange(0, list_rooms->length - 1));
    arrayList_free(list_rooms, NULL);

    return rand_cell;
}

static void set_treasure(Cell* cell){
    cell->type = TREASURE;
}


static void set_monster(Cell* cell){
    cell->type = MONSTER;
}

static void set_treasures(Map* map){
    static const int neighbors_dist1[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    int i, j;
    
    for (j = 0; j < HEIGHT; j++) {
        for (i = 0; i < WIDTH; i++){
            Cell* cell_treasure = &map->map[j][i];

            if (cell_treasure->type == ROOM) {
                if (compute_neighbors(map, cell_treasure, neighbors_dist1, 4, ROOM) == 1){
                    set_treasure(cell_treasure);
                    Cell* cell_monster = select_closest_room(map, cell_treasure);
                    set_monster(cell_monster);
                }
            } 
        }
    }
}



static void set_elements(Map* map){
    Cell* upstair = &map->map[HEIGHT / 2][WIDTH / 2];
    Cell* treasure = select_closest_room(map, upstair);
    treasure->type = TREASURE;

    set_treasures(map);
}


void generate_stage(Map* map){
    ArrayList* toexpand = arrayList_new();

    init_adjacent(map, toexpand);
    expand_room(map, toexpand);
    fill_wall(map); 
    arrayList_free(toexpand, NULL);

    map->map[HEIGHT / 2][WIDTH / 2].type = STAIR_UP;


}

