#include "Core/Backup/load.h"
#include "Core/GameStates.h"

#include <stdlib.h>
#include <stdio.h>

static void read(FILE* f, int* value) {
    fread(value, sizeof(int), 1, f);
}

static void load_equipment(Equipment** equipment, FILE* file){
    int typeValue;
    read(file, &typeValue);
    EquipmentType type = (EquipmentType)typeValue;

    if (type == -1) return;

    *equipment = equipment_new(type);

    switch (type)
    {
    case ARMOR:
        read(file, &(*equipment)->armor.quality);
        break;
    case MAGICWAND:
        read(file, &(*equipment)->magic_wand.quality);
        break;
    case WEAPON:
        read(file, &(*equipment)->weapon.quality);
        break;

    default:
        break;
    }
}

static void load_potion(Potion** potion, FILE* file){
    int typeValue;
    read(file, &typeValue);
    PotionType type = (PotionType)typeValue;

    if (type == -1) return;


    *potion = potion_new(type);

    switch (type)
    {
    case ACCURACY:
        read(file, &(*potion)->accuracy.crit);
        read(file, &(*potion)->accuracy.duration);
        break;
    case EXPERIENCE:
        read(file, &(*potion)->experience.exp);
        read(file, &(*potion)->experience.duration);
        break;
    case HEALTH:
        read(file, &(*potion)->health.hp);
        break;
    case MAGIC:
        read(file, &(*potion)->magic.mp);
        break;
    case REGENERATION:
        read(file, &(*potion)->regeneration.hp);
        read(file, &(*potion)->regeneration.mp);
        read(file, &(*potion)->regeneration.duration);
        read(file, &(*potion)->regeneration.interval);
        read(file, &(*potion)->regeneration.countdown);
        break;

    default:
        break;
    }
}

static void load_item(Item** item, FILE* file) {
    int typeValue;
    read(file, &typeValue);
    ItemType type = (ItemType)typeValue;

    if (type == -1) return;

    *item = item_new(type);

    switch (type)
    {
    case POTION:
        load_potion(&(*item)->potion, file);
        break;
    case EQUIPMENT:
        load_equipment(&(*item)->equipment, file);
        break;
    
    default:
        break;
    }
}



static void load_inventory(Inventory* inventory, FILE* file){
    int i;
    
    for(i = 0; i < NB_ITEMS; i++){
        load_item(&inventory->items[i], file);    
    }
}

static void load_bonus(Bonus* bonus, FILE* file){
    load_potion(&bonus->accuracy, file);
    load_potion(&bonus->experience, file);
    load_potion(&bonus->regeneration, file);
}

static void load_player(Player* player, FILE* file){
    read(file, &player->hp);
    read(file, &player->mp);
    read(file, &player->atk);
    read(file, &player->intel);
    read(file, &player->def);
    read(file, &player->exp);
    read(file, &player->lvl);
    read(file, &player->crit);
    read(file, &player->skill_points);

    fread(&player->position.x, sizeof(double), 1, file);
    fread(&player->position.y, sizeof(double), 1, file);

    
    load_inventory(&player->inventory, file);
    load_bonus(&player->bonus, file);
    
    auto_equip(player);
}

static void load_monster(Monster* monster, FILE* file){
    read(file, &monster->max_hp);
    read(file, &monster->hp);
    read(file, &monster->atk);
    read(file, &monster->crit);
    read(file, &monster->exp_given);
}

static void load_treasure(Treasure* treasure, FILE* file){
    int i;

    treasure->state = CLOSE;

    for (i = 0; i < ITEMS_PER_TREASURE; i++){
        load_item(&treasure->items[i], file);
    }
}

static void load_map(Map* map, FILE* file){
    int i, j;

    for (j = 0; j < HEIGHT; j++){
        for (i = 0; i < WIDTH; i++){
            Cell* cell = get_cell(map, i, j);

            int typeValue;
            read(file, &typeValue);
            cell->type = (Celltype)typeValue;
            
            switch (cell->type)
            {
            case MONSTER:
                load_monster(&cell->monster, file);
                break;
            case TREASURE:
                load_treasure(&cell->treasure, file);
                break;

            default:
                break;
            }
        }
    }

    update_stair_down(map);
}

static void load_maps(ArrayList* maps, FILE* file) {
    int i;

    /* clear existing map */
    Map* map = (Map*)arrayList_remove(maps, 0);
    map_free(map);

    int mapLength;
    read(file, &mapLength);

    for (i = 0; i < mapLength; i++){
        Map* map = map_new();
        load_map(map, file);
        arrayList_add(maps, map);
    }
}

void load_gamestates(char* path, GameStates* gs) {
    init_game_states(gs);

    FILE *file = fopen(path, "rb");

    if (file == NULL){
        perror("Load error !");
        return; /* Create a new Game if an error occurred */
    }

    Player* player = get_player(gs);

    load_player(player, file);
    read(file, &gs->current_stage);
    load_maps(gs->maps, file);

    fclose(file);
}
