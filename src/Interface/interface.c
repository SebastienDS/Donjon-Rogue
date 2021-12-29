#include <MLV/MLV_all.h>
#include <stdio.h>

#include "Interface/interface.h"
#include "Interface/View.h"
#include "Core/GameStates.h"
#include "Entity/Player.h"
#include "Map/Map.h"
#include "Util/ArrayList.h"
#include "constants.h"


void draw_interface(GameStates* gs, View* view) {
    Player* player = get_player(gs);
    int hp_max = get_hp_player(player);
    int mp_max = get_mp_player(player);

    draw_bar(15, 15, 400, 50, MLV_COLOR_GREEN1, player->hp, hp_max);
    draw_bar(15, 80, 400, 50, MLV_COLOR_CYAN3, player->mp, mp_max);

    print_floor(gs->current_stage, view->font);
    print_actions(gs, view->font);
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

    int coord_x = player->position.x;
    int coord_y = player->position.y;
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

void draw_mini_map(GameStates* gs, View* view, int start_x, int start_y, int cell_size) {
    Player* player = get_player(gs);
    Map* map = get_current_map(gs);

    int p_x = player->position.x;
    int p_y = player->position.y;

    static int width = VISION_X / 2;
    static int height = VISION_Y / 2;

    int offset_x = cell_size * (p_x - width);
    int offset_y = cell_size * (p_y - height);

    int i, j;

    for (j = -height; j <= height; j++) {
        for (i = -width; i <= width; i++) {
            int x = p_x + i;
            int y = p_y + j;

            if (!is_on_the_grid(x, y)) {
                continue;
            }

            Cell* cell = get_cell(map, x, y);

            MLV_Color color;

            if (cell->type == WALL) {
                color = MLV_COLOR_BLACK;
            } 
            else if (cell->type == TREASURE) {
                color = MLV_COLOR_GREEN;
            } 
            else if (cell->type == MONSTER) {
                color = MLV_COLOR_BLUE;
            } 
            else if (cell->type == ROOM){
                color = MLV_COLOR_WHITE;
            }
            else if (cell->type == STAIR_UP || cell->type == STAIR_DOWN) {
                color = MLV_COLOR_DEEP_PINK;
            }
            else {
                color = MLV_COLOR_RED;
            }

            MLV_draw_filled_rectangle(start_x + (x * cell_size - offset_x), start_y + (y * cell_size - offset_y), cell_size, cell_size, color);
        }
    }

    int player_pos_x = width * cell_size;
    int player_pos_y = height * cell_size;

    MLV_draw_filled_rectangle(start_x + player_pos_x, start_y + player_pos_y, cell_size, cell_size, MLV_COLOR_YELLOW);

    int dirX = start_x + player_pos_x + cell_size/2;
    int dirY = start_y + player_pos_y + cell_size/2;
    MLV_draw_line(dirX, dirY, dirX + player->direction.x * 15, dirY + player->direction.y * 15, MLV_COLOR_RED);
}