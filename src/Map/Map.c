#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "Map/Map.h"
#include "Util/ArrayList.h"
#include "Util/random.h"
#include "constants.h"
#include "Entity/Monster.h"
#include "Entity/Treasure.h"


Cell* get_cell(Map* map, int x, int y) {
    return &map->map[y][x];
}

Map* map_new() {
    Map* map = (Map*)malloc(sizeof(Map));
    if (map == NULL) {
        fprintf(stderr, "Map malloc error\n");
        exit(1);
    }

    init_map(map);
    generate_stage(map);

    return map;
}

void map_free(Map* map) {
    free(map);
}

void init_map(Map* self) {
    int i, j;

    for (j = 0; j < HEIGHT; j++) {
        for (i = 0; i < WIDTH; i++) {
            self->map[j][i].type = WALL;
            self->map[j][i].x = i;
            self->map[j][i].y = j;
        }
    }
    
    get_cell(self, START_X, START_Y)->type = ROOM;
}

bool is_on_the_grid(int x, int y) {
    return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
}

static int compute_neighbors(Map* map, Cell* cell, const int neighbors[][2], int length, Celltype type) {
    int i, pos_x, pos_y;
    int cpt = 0;
    
    for (i = 0; i < length; i++) {
        pos_x = neighbors[i][0] + cell->x;
        pos_y = neighbors[i][1] + cell->y;

        if (is_on_the_grid(pos_x, pos_y) && get_cell(map, pos_x, pos_y)->type == type) {
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

static bool is_eligible(Map* map, Cell* cell) {
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

        Cell* cell = get_cell(map, pos_x, pos_y);
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
        Cell* new_cell = get_cell(map, pos_x, pos_y);

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

    for (j = 0; j < HEIGHT; j++) {
        for (i = 0; i < WIDTH; i++) {
            Cell* cell = get_cell(map, i, j);

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

static void found_treasures(Map* map, ArrayList* treasures) {
    static const int neighbors_dist1[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    int i, j;

    for (j = 0; j < HEIGHT; j++) {
        for (i = 0; i < WIDTH; i++){
            Cell* cell_treasure = get_cell(map, i, j);

            if (cell_treasure->type == ROOM) {
                if (compute_neighbors(map, cell_treasure, neighbors_dist1, 4, ROOM) == 1){
                    arrayList_add(treasures, cell_treasure);
                }
            } 
        }
    }
}

static Cell* select_closest_room(Map* map, Cell* cell){
    static const int neighbors_dist1[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    ArrayList* list_rooms = arrayList_new();
    int i, pos_x, pos_y;
    
    for (i = 0; i < 4; i++) {
        pos_x = neighbors_dist1[i][0] + cell->x;
        pos_y = neighbors_dist1[i][1] + cell->y;
        Cell* new_cell = get_cell(map, pos_x, pos_y);

        if (new_cell->type == ROOM) {
            arrayList_add(list_rooms, new_cell);
        } 
    }

    Cell* closest_cell = NULL;

    /* 
        Prend en compte le cas ou il y a plusieurs coffres adjacent relié par 1 ROOM (qui devriendra MONSTER)
        On decide de garder ici seulement 1 coffre pour ne pas surcharger le monstre
    */
    if (list_rooms->length > 0) {
        int rand_index = randrange(0, list_rooms->length - 1);
        closest_cell = arrayList_get(list_rooms, rand_index);
    }

    arrayList_free(list_rooms, NULL);

    return closest_cell;
}

static void set_treasure(Cell* cell){
    cell->type = TREASURE;
    cell->treasure.state = CLOSE;
    init_treasure(&cell->treasure);
}


static void set_monster(Cell* cell){
    cell->type = MONSTER;
    init_monster(&cell->monster);
}

static void set_treasures(Map* map){
    int i;
    ArrayList* treasures = arrayList_new();
    
    found_treasures(map, treasures);

    for (i = 0; i < treasures->length; i++) {
        Cell* cell_treasure = arrayList_get(treasures, i);
        Cell* cell_monster = select_closest_room(map, cell_treasure);

        if (cell_monster == NULL) {
            continue;
        }
        
        set_treasure(cell_treasure);
        set_monster(cell_monster);
    }

    arrayList_free(treasures, NULL);
}

static void set_elements(Map* map){
    set_treasures(map);

    Cell* upstair = get_cell(map, START_X, START_Y);
    Cell* treasure = select_closest_room(map, upstair);
    set_treasure(treasure);
}

static int manhattan_distance(int i1, int j1, int i2, int j2) {
    return abs(i1 - i2) + abs(j1 - j2);
}

static void set_stair_down(Map* map, Cell* stair_up) {
    ArrayList* list = arrayList_new();

    int i, j;
    for (j = 0; j < HEIGHT; j++) {
        for (i = 0; i < WIDTH; i++) {
            Cell* cell = get_cell(map, i, j);

            if (cell->type != ROOM) continue;

            if (manhattan_distance(i, j, stair_up->x, stair_up->y)) { /* TODO: distance >= xxxx */
                arrayList_add(list, cell);
            }
        }
    }

    int index = randrange(0, list->length - 1);
    Cell* stair_down = (Cell*)arrayList_get(list, index);

    stair_down->type = STAIR_DOWN;
    printf("%d %d\n", stair_down->x, stair_down->y);

    arrayList_free(list, NULL);
}


void generate_stage(Map* map){
    ArrayList* toexpand = arrayList_new();

    init_adjacent(map, toexpand);
    expand_room(map, toexpand);
    fill_wall(map); 
    arrayList_free(toexpand, NULL);

    set_elements(map);
    Cell* cell = get_cell(map, START_X, START_Y);
    cell->type = STAIR_UP;

    set_stair_down(map, cell);
}

static bool can_move(Map* map, Player* player, int dx, int dy) {
    int x = player->pos.x + dx;
    int y = player->pos.y + dy;

    if (!is_on_the_grid(x, y)) return false;

    Cell* cell = get_cell(map, x, y);
    return cell->type == ROOM || (cell->type == TREASURE && cell->treasure.state == OPEN) || cell->type == STAIR_DOWN || cell->type == STAIR_UP;
}

static void move(Map* map, Player* player, int dx, int dy) {
    player->pos.x += dx;
    player->pos.y += dy;
}

bool try_move(Map* map, Player* player, int dx, int dy) {
    if (!can_move(map, player, dx, dy)) return false;

    move(map, player, dx, dy);
    return true;
}