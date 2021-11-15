#include <stdbool.h>
#include <stdlib.h>

#include "Map/Map.h"
#include "Util/ArrayList.h"
#include "Util/random.h"

static int NEIGHBOURS_DIST1[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
static int NEIGHBOURS_DIST2[8][2] = {{0, 2}, {0, -2}, {2, 0}, {-2, 0}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}};



void init_map(Map* self){
    int i, j;

    for (j = 0; j < HEIGHT; j++){
        for (i = 0; i < WIDTH; i++){
            self->map[j][i].type = WALL;
            self->map[j][i].x = i;
            self->map[j][i].y = j;
        }
    }
    self->map[j / 2][i / 2].type = STAIR_UP;
}

static bool distance_1(Map* map, int x, int y){
    int i, cpt = 0, pos_x, pos_y;
    

    for (i = 0; i < 4; i++){
        int direction[2] = NEIGHBOURS_DIST1[i];

        pos_x = direction[0] + x;
        pos_y = direction[1] + y;
        if (map->map[pos_y][pos_x].type == ROOM){
            cpt++;
        }
    }
    return cpt == 1;
}

static bool distance_2(Map* map, int x, int y){
     int i, cpt = 0, pos_x, pos_y;

    for (i = 0; i < 8; i++){
        int direction[2] = NEIGHBOURS_DIST2[i];

        pos_x = direction[0] + x;
        pos_y = direction[1] + y;
        if (map->map[pos_y][pos_x].type == ROOM){
            cpt++;
        }
    }
    return cpt <= 2;

}

bool is_eligible(Map* map, Cell * cell){
    return map->map[cell->y][cell->x].type == WALL
        && distance_1(map, cell->x, cell->y)
        && distance_2(map, cell->x, cell->y);
}

static void init_adjacent(Map* map, ArrayList* list){
    int i, pos_x, pos_y;
    int x_center = WIDTH / 2;
    int y_center = HEIGHT / 2;

    for(i = 0; i < 4; i++){
        int direction[2] = NEIGHBOURS_DIST1[i];

        pos_x = direction[0] + x_center;
        pos_y = direction[1] + y_center;

        Cell* cell = &(map->map[pos_y][pos_x]);
        arrayList_add(list, cell);
    }

}

bool is_on_the_grid(int x, int y){
    return x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT;
}

static void fill_adjacent(Map* map, ArrayList* list, Cell* cell){
    int i, pos_x, pos_y;

    for (i = 0; i < 4; i++){
        int direction[2] = NEIGHBOURS_DIST1[i];

        pos_x = direction[0] + cell->x;
        pos_y = direction[1] + cell->y;
        Cell* new_cell = &(map->map[pos_y][pos_x]);


        if (is_eligible(map, new_cell) && !is_on_the_grid(pos_x, pos_y) && !arrayList_contains(map, new_cell)){
            arrayList_add(list, new_cell);
        }
    }
}

static void fill_wall(Map* map){
    int i, j, k, l, cpt = 0, pos_x, pos_y;
    ArrayList* room_to_wall = arrayList_new();
    
    for (j = 0; j < HEIGHT; j++){
        for (i = 0; i < WIDTH; i++){
            cpt = 0;
            Cell* cell;

            if (map->map[pos_y][pos_x].type == ROOM){
                for (k = 0; i < 4; i++){
                    int direction[2] = NEIGHBOURS_DIST1[i];

                    pos_x = direction[0] + i;
                    pos_y = direction[1] + j;
                    cell = &(map->map[pos_y][pos_x]);
                    if (cell->type == WALL){
                        cpt++;
                    }
                }
            }
            if (cpt >= 3){
                arrayList_add(room_to_wall, cell);
            }
        }
    }
    for (l = 0; l < room_to_wall->length; l++){
        Cell* cell = (Cell*) arrayList_get(room_to_wall, l);

        cell->type = WALL;
    }
    arrayList_free(room_to_wall, NULL);
}


void generate_stage(Map* map){
    ArrayList* toexpand = arrayList_new();
    init_adjacent(map, toexpand);
    Cell* cell;

    do{
        do {
            int index = randrange(0, toexpand->length - 1);
            cell = (Cell*) arrayList_remove(toexpand, index);

        } while (!is_eligible(map, cell));
        
        cell->type = ROOM;
        fill_adjacent(map, toexpand, cell);
    } while (toexpand->length != 0);
    fill_wall(map);

    arrayList_free(toexpand, NULL);
}

