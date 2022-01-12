#include <MLV/MLV_all.h>
#include <stdio.h>

#include "Interface/interface.h"
#include "Interface/View.h"
#include "Core/GameStates.h"
#include "Entity/Player.h"
#include"Entity/Inventory/Inventory.h"
#include "Entity/Inventory/Potions/Potion.h"
#include "Entity/Inventory/Equipments/Equipment.h"
#include "Entity/Treasure.h"
#include "constants.h"
#include <stdio.h>
#include "Map/Map.h"
#include "Util/ArrayList.h"
#include "constants.h"


void draw_bar(int pos_x, int pos_y, int size_x, int size_y, MLV_Color color, int value, int max){
    MLV_draw_filled_rectangle(pos_x, pos_y, size_x, size_y, MLV_COLOR_GRAY12);
    MLV_draw_filled_rectangle(pos_x, pos_y, value * size_x / max, size_y, color);

    char str[50];
    int width, height;

    sprintf(str, "%d / %d", value, max);
    MLV_get_size_of_text(str, &width, &height);
    MLV_draw_text(pos_x + ((size_x - width) / 2), pos_y + ((size_y - height) / 2), str, MLV_COLOR_WHITE_SMOKE);
}

void print_floor(int floor, MLV_Font* font){
    char str[50];
    int width, height;

    sprintf(str, "Floor - %d", floor);
    MLV_get_size_of_text_with_font(str, &width, &height, font);
    MLV_draw_text_with_font(SCREEN_WIDTH - width - 15, 15, str, font, MLV_COLOR_WHITE_SMOKE);
}

static void print_level(int level) {
    char str[50];
    int width, height;

    sprintf(str, "%d", level);
    MLV_get_size_of_text(str, &width, &height);
    MLV_draw_text((SCREEN_WIDTH - width) / 2, (SCREEN_HEIGHT - CELL_SIZE - height) / 2 - 10, str, MLV_COLOR_WHITE_SMOKE);
}

static void print_instruction(char key, char* text, MLV_Font* font, int y){
    char str[50];
    int width, height;

    sprintf(str, "%c - %s", key, text);
    MLV_get_size_of_text_with_font(str, &width, &height, font);
    MLV_draw_text_with_font(15, SCREEN_HEIGHT - (y + 1) * height, str, font, MLV_COLOR_WHITE_SMOKE);
}


void print_actions(GameStates* gs, MLV_Font* font){
    static const int neighbors[5][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {0, 0}};

    Map* map = get_current_map(gs);
    Player* player = get_player(gs);
    int cpt = 0;

    Cell* cell = get_cell(map, player->position.x, player->position.y);

    if (compute_neighbors(map, cell, neighbors, 5, STAIR_DOWN) || compute_neighbors(map, cell, neighbors, 5, STAIR_UP)) {
        print_instruction('E', "Use stair", font, cpt);
        cpt++;
    }
    if (compute_neighbors(map, cell, neighbors, 5, TREASURE)) {
        print_instruction('T', "Open treasure", font, cpt);
        cpt++;
    }
    if (compute_neighbors(map, cell, neighbors, 5, MONSTER)) {
        print_instruction('P', "Set physical attack", font, cpt);
        cpt++;
        print_instruction('M', "Set magical attack", font, cpt);
        cpt++;
        print_instruction('A', "Attack monster", font, cpt);
        cpt++;
    }
}

static void get_type_equipment(Player* player, Equipment* equipment, int i, int j, Inventory_icones* icones){
    static int start_x = SCREEN_WIDTH * 3 / 5 + 15;
    static int start_y = 30;
    static int width_items = (SCREEN_WIDTH * 2 / 5 - 45) / 4;

    switch (equipment->type)
    {
    case WEAPON:      
        if (player->bonus.weapon == equipment){
            MLV_draw_filled_rectangle(start_x + (i * width_items), start_y + (j * width_items), width_items, width_items, MLV_COLOR_GREEN);
        }

        MLV_draw_image(icones->sword, start_x + (i * width_items), start_y + (j * width_items));
        break;
    case ARMOR:
        if (player->bonus.armor == equipment){
            MLV_draw_filled_rectangle(start_x + (i * width_items), start_y + (j * width_items), width_items, width_items, MLV_COLOR_GREEN);
        }

        MLV_draw_image(icones->armor, start_x + (i * width_items), start_y + (j * width_items));
        break;
    case MAGICWAND:        
        if (player->bonus.magic_wand == equipment){
            MLV_draw_filled_rectangle(start_x + (i * width_items), start_y + (j * width_items), width_items, width_items, MLV_COLOR_GREEN);
        }

        MLV_draw_image(icones->magic_wand, start_x + (i * width_items), start_y + (j * width_items));
        break;
    default:
        break;
    }
}

static void get_type_potion(Player* player, Potion* potion, int i, int j, Inventory_icones* icones){
    static int start_x = SCREEN_WIDTH * 3 / 5 + 15;
    static int start_y = 30;
    static int width_items = (SCREEN_WIDTH * 2 / 5 - 45) / 4;

    switch (potion->type)
    {
    case HEALTH:
    case MAGIC:
        MLV_draw_image(icones->instant_potion, start_x + (i * width_items), start_y + (j * width_items));
        break;

    default:
        MLV_draw_image(icones->potion, start_x + (i * width_items), start_y + (j * width_items));
        break;
    }
}


static void draw_items(GameStates* gs, Inventory_icones* icones){
    int i, j;
    static int start_x = SCREEN_WIDTH * 3 / 5 + 15;
    static int start_y = 30;
    static int width_items = (SCREEN_WIDTH * 2 / 5 - 45) / 4;
    Player* player = get_player(gs);

    for (j = 0; j < 3; j++){
        for (i = 0; i < 4; i++){
            Item* item = player->inventory.items[i + j * 4];

            MLV_draw_rectangle(start_x + (i * width_items), start_y + (j * width_items), width_items, width_items, MLV_COLOR_BLACK);

            if (item == NULL) continue;

            switch (item->type)
            {
            case EQUIPMENT:
                get_type_equipment(player, item->equipment, i, j, icones);
                break;
            case POTION:
                get_type_potion(player, item->potion, i, j, icones);
                break;
            default:
                break;
            }
        }
    }
}

static void get_type_equipment_treasure(Equipment* equipment, int i, Inventory_icones* icones){
    int width_items = (SCREEN_WIDTH * 2 / 5 - 45) / 4;
    int start_x = 30 + width_items;
    int start_y = 30 + width_items;

    switch (equipment->type)
    {
    case WEAPON:      
        MLV_draw_image(icones->sword, start_x + (i * width_items), start_y);
        break;
    case ARMOR:
        MLV_draw_image(icones->armor, start_x + (i * width_items), start_y);
        break;
    case MAGICWAND:        
        MLV_draw_image(icones->magic_wand, start_x + (i * width_items), start_y);
        break;
    default:
        break;
    }
}

static void get_type_potion_treasure(Potion* potion, int i, Inventory_icones* icones){
    int width_items = (SCREEN_WIDTH * 2 / 5 - 45) / 4;
    int start_x = 30 + width_items;
    int start_y = 30 + width_items;

    switch (potion->type)
    {
    case HEALTH:
    case MAGIC:
        MLV_draw_image(icones->instant_potion, start_x + (i * width_items), start_y);
        break;
    default:
        MLV_draw_image(icones->potion, start_x + (i * width_items), start_y);
        break;
    }
}

static void draw_items_treasure(GameStates* gs, Inventory_icones* icones){
    int i;
    int width_items = (SCREEN_WIDTH * 2 / 5 - 45) / 4;
    int start_x = 30 + width_items;
    int start_y = 30 + width_items;
    Treasure* treasure = gs->treasure.treasure;

    for (i = 0; i < ITEMS_PER_TREASURE; i++){
        Item* item = treasure->items[i];

        MLV_draw_rectangle(start_x + (i * width_items), start_y, width_items, width_items, MLV_COLOR_BLACK);

        if (item == NULL) continue;

        switch (item->type)
        {
            case EQUIPMENT:
                get_type_equipment_treasure(item->equipment, i, icones);
                break;
            case POTION:
                get_type_potion_treasure(item->potion, i, icones);
                break;
            default:
                break;
        }        
    }
}

static void draw_stat_potion(Potion* potion, MLV_Font* font, int x, int y){
    char str[255];

    switch (potion->type)
    {
    case ACCURACY:
        sprintf(str, "Accuracy potion\nCrit : %d\nDuration : %d", potion->accuracy.crit, potion->accuracy.duration);
        break;
    case EXPERIENCE:
        sprintf(str, "Experience potion\nExp : %d\nDuration : %d", potion->experience.exp, potion->experience.duration);
        break;
    case HEALTH:
        sprintf(str, "Health potion\nHP : %d", potion->health.hp);
        break;
    case MAGIC:
        sprintf(str, "Magic potion\nMP : %d", potion->magic.mp);
        break;
    case REGENERATION:
        sprintf(str, "Regeneration potion\nHP : %d\nMP : %d\nDuration : %d\nInterval : %d", potion->regeneration.hp, potion->regeneration.mp, potion->regeneration.duration, potion->regeneration.interval);
        break;
    default:
        break;
    }
    /*MLV_draw_text(SCREEN_WIDTH * 3 / 5 + 15, SCREEN_HEIGHT - 30 - height, str, MLV_COLOR_WHITE_SMOKE);*/
    MLV_draw_text_box_with_font(x, y, SCREEN_WIDTH * 2 / 5 - 45, 330, str, font, 15, MLV_COLOR_GRAY2, MLV_COLOR_BLACK, MLV_COLOR_GRAY, MLV_TEXT_LEFT, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
}

static void draw_stat_equipment(Equipment* equipment, MLV_Font* font, int x, int y){
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
    MLV_draw_text_box_with_font(x, y, SCREEN_WIDTH * 2 / 5 - 45, 330, str, font, 15, MLV_COLOR_GRAY2, MLV_COLOR_BLACK, MLV_COLOR_GRAY, MLV_TEXT_LEFT, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
}


static void draw_stats(GameStates* gs, Item* item, MLV_Font* font){
   
    switch (item->type)
    {
    case POTION:
        draw_stat_potion(item->potion, font, SCREEN_WIDTH * 3 / 5 + 15, SCREEN_HEIGHT / 2 - 15);
        break;
    case EQUIPMENT:
        draw_stat_equipment(item->equipment, font, SCREEN_WIDTH * 3 / 5 + 15, SCREEN_HEIGHT / 2 - 15);
        break;
    
    default:
        break;
    }
}

static void draw_stats_treasure(GameStates* gs, Item* item, MLV_Font* font){
   
    switch (item->type)
    {
    case POTION:
        draw_stat_potion(item->potion, font, 30, SCREEN_HEIGHT / 2 - 15);
        break;
    case EQUIPMENT:
        draw_stat_equipment(item->equipment, font, 30, SCREEN_HEIGHT / 2 - 15);
        break;
    default:
        break;
    }
}



void draw_button(Button* button, MLV_Font* font) {
    MLV_draw_text_box_with_font(button->x, button->y, button->width, button->height, button->label, font, 15, MLV_COLOR_GRAY2, MLV_COLOR_BLACK, MLV_COLOR_GRAY, MLV_TEXT_LEFT, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
}

static void draw_inventory(GameStates* gs, View* view){
    Item* item = gs->inventory.item_selected;
    Player* player = get_player(gs);

    MLV_draw_filled_rectangle(SCREEN_WIDTH * 3 / 5, 15, SCREEN_WIDTH * 2 / 5 - 15, SCREEN_HEIGHT - 30, MLV_COLOR_GRAY);
    draw_items(gs, &view->inventory_icones);

    if (item == NULL) return;

    draw_stats(gs, item, view->font);
    if (item->type == POTION){
        draw_button(&gs->inventory.use, view->font);
    }

    if (verif_equiped(player, item)) return;

    draw_button(&gs->inventory.throw, view->font);
}

static void draw_treasure(GameStates* gs, View* view){
    Item* item = gs->treasure.item_selected;

    MLV_draw_filled_rectangle(15, 15, SCREEN_WIDTH * 2 / 5 - 15, SCREEN_HEIGHT - 30, MLV_COLOR_GRAY);
    draw_items_treasure(gs, &view->inventory_icones);

    draw_button(&gs->treasure.close, view->font);

    if (item == NULL) return;

    draw_stats_treasure(gs, item, view->font);
    draw_button(&gs->treasure.take, view->font);
}

static void draw_attack_icon(int x, int y, AttackType type, View* view) {
    if (type == PHYSICAL) MLV_draw_image(view->inventory_icones.sword_mode, x, y);
    else if (type == MAGICAL)  MLV_draw_image(view->inventory_icones.magic_wand_mode, x, y);
}

static void print_skill(int x, int y, char* text, int value, MLV_Font* font, int cpt) {
    char str[50];
    int width, height;

    sprintf(str, "%s - %d", text, value);
    MLV_get_size_of_text_with_font(str, &width, &height, font);
    MLV_draw_text_with_font(x, y + cpt * height, str, font, MLV_COLOR_WHITE_SMOKE);
}
    
static void draw_skills(SkillsUpgrades* btns, int x, int y, Player* player, MLV_Font* font) {
    print_skill(x, y, "Atk", player->atk, font, 0);
    print_skill(x, y, "Int", player->intel, font, 1);
    print_skill(x, y, "Def", player->def, font, 2);

    if (!player->skill_points) return;

    draw_button(&btns->atk, font);
    draw_button(&btns->intel, font);
    draw_button(&btns->def, font);

}


void draw_interface(GameStates* gs, View* view) {
    Player* player = get_player(gs);
    int hp_max = get_hp_player(player);
    int mp_max = get_mp_player(player);

    draw_bar(15, 15, 400, 50, MLV_COLOR_GREEN1, player->hp, hp_max);
    draw_bar(15, 80, 400, 50, MLV_COLOR_CYAN3, player->mp, mp_max);
    draw_bar(15, 145, 400, 25, MLV_COLOR_GRAY, player->exp, required_experience(player->lvl));

    draw_attack_icon(430, 15, player->attackType, view);
    draw_skills(&gs->skills_btn, 15, 185, player, view->medium_font);
    
    if (gs->viewType == DEFAULT) print_level(player->lvl);
    
    print_floor(gs->current_stage, view->font);
    print_actions(gs, view->font);

    if (gs->inventory.is_open) {
        draw_inventory(gs, view);
    }
    else {
        MLV_draw_image(view->images.treasure.open, SCREEN_WIDTH - CELL_SIZE - 15, SCREEN_HEIGHT - CELL_SIZE - 15);
    }
    if (gs->treasure.is_open) draw_treasure(gs, view);
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

            if (!is_on_the_grid(x, y)) continue;

            Cell* cell = get_cell(map, x, y);

            MLV_Color color;

            if (cell->type == WALL) color = MLV_COLOR_BLACK; 
            else if (cell->type == TREASURE) color = MLV_COLOR_GREEN; 
            else if (cell->type == MONSTER) color = MLV_COLOR_BLUE;
            else if (cell->type == ROOM) color = MLV_COLOR_WHITE;
            else if (cell->type == STAIR_UP || cell->type == STAIR_DOWN) color = MLV_COLOR_DEEP_PINK;
            else color = MLV_COLOR_RED;

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
