#include "Core/save.h"
#include "Core/GameStates.h"

#include <stdlib.h>
#include <stdio.h>

static void save_equipment(Equipment* equipment, FILE* file){
    if (equipment == NULL) {
        fprintf(file, "-1;");
        return;
    }

    switch (equipment->type)
    {
    case ARMOR:
        fprintf(file, "%d;%d;", equipment->type, equipment->armor.quality);
        break;
    case MAGICWAND:
        fprintf(file, "%d;%d;", equipment->type, equipment->magic_wand.quality);
        break;
    case WEAPON:
        fprintf(file, "%d;%d;", equipment->type, equipment->weapon.quality);
        break;

    default:
        break;
    }
}

static void save_potion(Potion* potion, FILE* file){
    if (potion == NULL) {
        fprintf(file, "-1;");
        return;
    }

    switch (potion->type)
    {
    case ACCURACY:
        fprintf(file, "%d;%d;", potion->accuracy.crit, potion->accuracy.duration);
        break;
    case EXPERIENCE:
        fprintf(file, "%d;%d;", potion->experience.exp, potion->experience.duration);
        break;
    case HEALTH:
        fprintf(file, "%d;", potion->health.hp);
        break;
    case MAGIC:
        fprintf(file, "%d;", potion->magic.mp);
        break;
    case REGENERATION:
        fprintf(file, "%d;%d;%d;%d;%d;", potion->regeneration.hp, potion->regeneration.mp, potion->regeneration.duration, potion->regeneration.interval, potion->regeneration.countdown);
        break;

    default:
        break;
    }
}

static void save_item(Item* item, FILE* file){
    if (item == NULL) {
        fprintf(file, "-1;");
        return;
    }

    switch (item->type)
    {
    case POTION:
        save_potion(item->potion, file);
        break;
    case EQUIPMENT:
        save_equipment(item->equipment, file);
        break;
    
    default:
        break;
    }
}



static void save_inventory(Inventory* inventory, FILE* file){
    int i;
    
    for(i = 0; i < NB_ITEMS; i++){
        save_item(inventory->items[i], file);    
    }
}

static void save_bonus(Bonus* bonus, FILE* file){
    save_potion(bonus->accuracy, file);
    save_potion(bonus->experience, file);
    save_potion(bonus->regeneration, file);
}

static void save_player(Player* player, FILE* file){
    fprintf(file, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%f;%f;", 
        player->hp,
        player->mp,
        player->atk,
        player->intel,
        player->def,
        player->exp,
        player->lvl,
        player->crit,
        player->skill_points,
        player->position.x,
        player->position.y);
    
    save_inventory(&player->inventory, file);
    save_bonus(&player->bonus, file);
}

static void save_monster(Monster* monster, FILE* file){
    fprintf(file, "%d;%d;%d;%d;%d;", 
        monster->max_hp, 
        monster->hp,   
        monster->atk, 
        monster->crit,
        monster->exp_given);
}

static void save_treasure(Treasure* treasure, FILE* file){
    int i;

    for (i = 0; i < ITEMS_PER_TREASURE; i++){
        save_item(treasure->items[i], file);
    }
}

static void save_map(Map* map, FILE* file){
    int i, j;

    for (j = 0; j < HEIGHT; j++){
        for (i = 0; i < WIDTH; i++){
            Cell* cell = get_cell(map, i, j);

            switch (cell->type)
            {
            case MONSTER:
                fprintf(file, "%d;", cell->type);
                save_monster(&cell->monster, file);
                break;
            case TREASURE:
                if (cell->treasure.state == CLOSE) {
                    fprintf(file, "%d;", cell->type);
                    save_treasure(&cell->treasure, file);
                }
                else {
                    fprintf(file, "%d;", ROOM);
                }
                break;
            
            default:
                fprintf(file, "%d;", cell->type);
                break;
            }
        }
    }
}

static void save_maps(ArrayList* maps, FILE* file){
    int i;

    fprintf(file, "%d;", maps->length);
    for (i = 0; i < maps->length; i++){
        save_map(arrayList_get(maps, i), file);
    }
}

void save_gamestates(GameStates* gs, char* path){
    FILE *fic = fopen(path, "wb");

    if (fic == NULL){
        perror("Save error !");
        exit(1);
    }

    Player* player = get_player(gs);

    save_player(player, fic);
    fprintf(fic, "%d;", gs->current_stage);
    save_maps(gs->maps, fic);

    fclose(fic);
}




GameStates* load_gamestates(char* path){
    return NULL;
}
