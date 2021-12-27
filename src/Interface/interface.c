#include "Interface/interface.h"
#include "Core/GameStates.h"
#include <MLV/MLV_all.h>
#include "Entity/Player.h"
#include"Entity/Inventory/Inventory.h"
#include "Entity/Inventory/Potions/Potion.h"
#include "Entity/Inventory/Equipments/Equipment.h"
#include "constants.h"
#include <stdio.h>
#include "Map/Map.h"
#include "Util/ArrayList.h"

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

static void draw_items(GameStates* gs){
    int i, j;
    int start_x = SCREEN_WIDTH * 3 / 5 + 15;
    int start_y = 30;
    int width_items = (SCREEN_WIDTH * 2 / 5 - 45) / 4;

    for (j = 0; j < 3; j++){
        for (i = 0; i < 4; i++){
            MLV_draw_rectangle(start_x + (i * width_items), start_y + (j * width_items), width_items, width_items, MLV_COLOR_MAGENTA);
        }
    }
}

static void draw_stat_potion(Potion* potion, MLV_Font* font){
    char str[255];

    switch (potion->type)
    {
    case ACCURACY:
        sprintf(str, "Accuracy potion : \nCrit : %d\nDuration : %d", potion->accuracy.crit, potion->accuracy.duration);
        break;
    case EXPERIENCE:
        sprintf(str, "Experience potion : \nExp : %d\nDuration : %d", potion->experience.exp, potion->experience.duration);
        break;
    case HEALTH:
        sprintf(str, "Health potion : \nHP : %d", potion->health.hp);
        break;
    case MAGIC:
        sprintf(str, "Magic potion : \nMP : %d", potion->magic.mp);
        break;
    case REGENERATION:
        sprintf(str, "Regeneration potion : \nHP : %d\nMP : %d\nDuration : %d\nInterval : %d", potion->regeration.hp, potion->regeration.mp, potion->regeration.duration, potion->regeration.interval);
        break;
    default:
        break;
    }
    /*MLV_draw_text(SCREEN_WIDTH * 3 / 5 + 15, SCREEN_HEIGHT - 30 - height, str, MLV_COLOR_WHITE_SMOKE);*/
    MLV_draw_text_box_with_font(SCREEN_WIDTH * 3 / 5 + 15, SCREEN_HEIGHT / 2, SCREEN_WIDTH * 2 / 5 - 45, 300, str, font, 15, MLV_COLOR_GRAY2, MLV_COLOR_MAGENTA, MLV_COLOR_GRAY, MLV_TEXT_LEFT, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
}

static void draw_stat_equipment(Equipment* equipment, MLV_Font* font){
    char str[255];

    switch (equipment->type)
    {
    case ARMOR:
        sprintf(str, "Armor : %d", equipment->armor.quality);
        break;
    case MAGICWAND:
        sprintf(str, "Magic wand : %d", equipment->magic_wand.quality);
        break;
    case WEAPON:
        sprintf(str, "Weapon : %d", equipment->weapon.quality);
        break;
    default:
        break;
    }
    /*MLV_draw_text(SCREEN_WIDTH * 3 / 5 + 15, SCREEN_HEIGHT - 30 - height, str, MLV_COLOR_WHITE_SMOKE);*/
    MLV_draw_text_box_with_font(SCREEN_WIDTH * 3 / 5 + 15, SCREEN_HEIGHT / 2, SCREEN_WIDTH * 2 / 5 - 45, 300, str, font, 15, MLV_COLOR_GRAY, MLV_COLOR_MAGENTA, MLV_COLOR_GRAY, MLV_TEXT_LEFT, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
}


static void draw_stats(GameStates* gs, MLV_Font* font){
    Item* item = get_player(gs)->inventory.items[0];

    switch (item->type)
    {
    case POTION:
        draw_stat_potion(item->potion, font);
        break;
    case EQUIPMENT:
        draw_stat_equipment(item->equipment, font);
        break;
    
    default:
        break;
    }
}


static void draw_inventory(GameStates* gs, MLV_Font* font){
    MLV_draw_filled_rectangle(SCREEN_WIDTH * 3 / 5, 15, SCREEN_WIDTH * 2 / 5 - 15, SCREEN_HEIGHT - 30, MLV_COLOR_GRAY);
    draw_items(gs);
    draw_stats(gs, font);
}



void draw_interface(GameStates* gs, MLV_Font* font) {
    Player* player = get_player(gs);
    int hp_max = get_hp_player(player);
    int mp_max = get_mp_player(player);

    draw_bar(15, 15, 400, 50, MLV_COLOR_GREEN1, player->hp, hp_max);
    draw_bar(15, 80, 400, 50, MLV_COLOR_CYAN3, player->mp, mp_max);
    print_floor(gs->current_stage, font);
    print_actions(gs, font);

    if(gs->inventory_is_open) draw_inventory(gs, font);
}


