#include "Interface/interface.h"
#include "Core/GameStates.h"
#include <MLV/MLV_all.h>
#include "Entity/Player.h"
#include "constants.h"
#include <stdio.h>
#include "Map/Map.h"
#include "Util/ArrayList.h"

void draw_interface(GameStates* gs, MLV_Font* font) {
    Player* player = get_player(gs);
    int hp_max = get_hp_player(player);
    int mp_max = get_mp_player(player);

    draw_bar(15, 15, 400, 50, MLV_COLOR_GREEN1, player->hp, hp_max);
    draw_bar(15, 80, 400, 50, MLV_COLOR_CYAN3, player->mp, mp_max);
    print_floor(gs->current_stage, font);
    print_actions(gs, font);
}

void draw_bar(int pos_x, int pos_y, int size_x, int size_y, MLV_Color color, int value, int max){
    MLV_draw_filled_rectangle(pos_x, pos_y, size_x, size_y, MLV_COLOR_GRAY12);
    MLV_draw_filled_rectangle(pos_x, pos_y, value * size_x / max, size_y, color);
}

void print_floor(int floor, MLV_Font* font){
    char str[50];
    int width, height;

    sprintf(str, "Floor - %d", floor);
    MLV_get_size_of_text_with_font(str, &width, &height, font);
    MLV_draw_text_with_font(SCREEN_WIDTH - width - 15, 15, str, font, MLV_COLOR_WHITE_SMOKE);
}

static void get_possible_action(GameStates* gs, ArrayList* list) {
    static const int neighbors_dist1[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    Player* player = get_player(gs);
    Map* map = get_current_map(gs);

    int coord_x = player->pos.x;
    int coord_y = player->pos.y;
    int i, pos_x, pos_y;

    Cell* cell = get_cell(map, coord_x, coord_y);

    if (cell->type == STAIR_DOWN || (cell->type == STAIR_UP && gs->current_stage != 0)){
        arrayList_add(list, cell);
    }

    for (i = 0; i < 4; i++) {
        pos_x = neighbors_dist1[i][0] + coord_x;
        pos_y = neighbors_dist1[i][1] + coord_y;

        Cell* cell = get_cell(map, pos_x, pos_y);

        if ((cell->type == TREASURE && cell->treasure.state == CLOSE) || cell->type == MONSTER){
            arrayList_add(list, cell);
        }
    }
}

static void print_instruction(char key, char* text, MLV_Font* font, int y){
    char str[50];
    int width, height;

    sprintf(str, "%s - %c", text, key);
    MLV_get_size_of_text_with_font(str, &width, &height, font);
    MLV_draw_text_with_font(15, SCREEN_HEIGHT - (y + 1) * height, str, font, MLV_COLOR_WHITE_SMOKE);
}


void print_actions(GameStates* gs, MLV_Font* font){
    ArrayList* list = arrayList_new();
    int i;

    get_possible_action(gs, list);

    for (i = 0; i < list->length; i++){
        Cell* cell = arrayList_get(list, i);

        switch (cell->type)
        {
            case STAIR_UP:
            case STAIR_DOWN:
                print_instruction('E', "Use stair", font, i);
                break;

            case TREASURE:  
                print_instruction('T', "Open treasure", font, i);
                break;
                
            case MONSTER:
                print_instruction('A', "Attack monster", font, i);
                print_instruction('M', "Attack monster with magic", font, i + 1);
                break;
            
            default:
                break;
        }
    }
    arrayList_free(list, NULL);
}

