#include "Core/Backup/save.h"
#include "Core/GameStates.h"

#include <stdlib.h>
#include <stdio.h>

static void write(FILE* f, int value) {
    fwrite(&value, sizeof(int), 1, f);
}

static void save_equipment(Equipment* equipment, FILE* file){
    if (equipment == NULL) {
        write(file, -1);
        return;
    }

    write(file, equipment->type);

    switch (equipment->type)
    {
    case ARMOR:
        write(file, equipment->armor.quality);
        break;
    case MAGICWAND:
        write(file, equipment->magic_wand.quality);
        break;
    case WEAPON:
        write(file, equipment->weapon.quality);
        break;

    default:
        break;
    }
}

static void save_potion(Potion* potion, FILE* file){
    if (potion == NULL) {
        write(file, -1);
        return;
    }

    write(file, potion->type);

    switch (potion->type)
    {
    case ACCURACY:
        write(file, potion->accuracy.crit);
        write(file, potion->accuracy.duration);
        break;
    case EXPERIENCE:
        write(file, potion->experience.exp);
        write(file, potion->experience.duration);
        break;
    case HEALTH:
        write(file, potion->health.hp);
        break;
    case MAGIC:
        write(file, potion->magic.mp);
        break;
    case REGENERATION:
        write(file, potion->regeneration.hp);
        write(file, potion->regeneration.mp);
        write(file, potion->regeneration.duration);
        write(file, potion->regeneration.interval);
        write(file, potion->regeneration.countdown);
        break;

    default:
        break;
    }
}

static void save_item(Item* item, FILE* file){
    if (item == NULL) {
        write(file, -1);
        return;
    }
        
    write(file, item->type);

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
    write(file, player->hp);
    write(file, player->mp);
    write(file, player->atk);
    write(file, player->intel);
    write(file, player->def);
    write(file, player->exp);
    write(file, player->lvl);
    write(file, player->crit);
    write(file, player->skill_points);

    fwrite(&player->position.x, sizeof(double), 1, file);
    fwrite(&player->position.y, sizeof(double), 1, file);

    
    save_inventory(&player->inventory, file);
    save_bonus(&player->bonus, file);
}

static void save_monster(Monster* monster, FILE* file){
    write(file, monster->max_hp);
    write(file, monster->hp);
    write(file, monster->atk);
    write(file, monster->crit);
    write(file, monster->exp_given);
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
                write(file, cell->type);
                save_monster(&cell->monster, file);
                break;
            case TREASURE:
                if (cell->treasure.state == CLOSE) {
                    write(file, cell->type);
                    save_treasure(&cell->treasure, file);
                }
                else {
                    write(file, ROOM);
                }
                break;
            
            default:
                write(file, cell->type);
                break;
            }
        }
    }
}

static void save_maps(ArrayList* maps, FILE* file){
    int i;

    write(file, maps->length);

    for (i = 0; i < maps->length; i++){
        save_map(arrayList_get(maps, i), file);
    }
}

void save_gamestates(GameStates* gs, char* path){
    FILE *file = fopen(path, "wb");

    if (file == NULL){
        perror("Save error !");
        exit(1);
    }

    Player* player = get_player(gs);

    save_player(player, file);
    write(file, gs->current_stage);
    save_maps(gs->maps, file);

    fclose(file);
}