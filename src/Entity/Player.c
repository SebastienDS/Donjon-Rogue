#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "Entity/Player.h"
#include "Entity/Monster.h"
#include "Entity/Inventory/Inventory.h"
#include "Entity/Inventory/Potions/potionEffect.h"
#include "Map/Map.h"
#include "Util/random.h"
#include "constants.h"


#define CRIT_MULTIPLICATOR 3

void init_player(Player* self, int x, int y) {
    self->attackType = PHYSICAL;

    self->position.x = x;
    self->position.y = y;

    self->direction.x = -1;
    self->direction.y = 0;

    self->plane.x = 0;
    self->plane.y = -0.66;
    rotate_player(self, M_PI / 2.0);

    self->bonus.regeneration = NULL;
    self->bonus.accuracy = NULL;
    self->bonus.experience = NULL;
    self->bonus.weapon = NULL;
    self->bonus.magic_wand = NULL;
    self->bonus.armor = NULL;

    self->atk = 10;
    self->intel = 10;
    self->def = 10;
    self->crit = 5;
    self->lvl = 1;
    self->exp = 0;
    self->hp = get_hp_player(self); 
    self->mp = get_mp_player(self);

    self->skill_points = 0;

    init_inventory(&self->inventory);
    self->inventory.items[0] = get_random_item(1);
    self->inventory.items[2] = get_random_item(1);
    self->inventory.items[3] = get_random_item(1);
    self->inventory.items[4] = get_random_item(1);
    self->inventory.items[5] = get_random_item(1);
    self->inventory.items[6] = get_random_item(1);
    self->inventory.items[7] = get_random_item(1);
    self->inventory.items[8] = get_random_item(1);
    self->inventory.items[9] = get_random_item(1);
    self->inventory.items[10] = get_random_item(1);
    self->inventory.items[11] = get_random_item(1);

    auto_equip(self);
}

int get_hp_player(Player* self){
    return 10 * get_def(self);
}

int get_mp_player(Player* self){
    return 10 * get_intel(self) - 50;
}

int get_atk(Player* self) {
    int bonus = self->bonus.weapon == NULL ? 0 : self->bonus.weapon->weapon.quality; 
    return self->atk + bonus;
}

int get_def(Player* self) {
    int bonus = self->bonus.armor == NULL ? 0 : self->bonus.armor->armor.quality; 
    return self->def + bonus;
}

int get_intel(Player* self) {
    int bonus = self->bonus.weapon == NULL ? 0 : self->bonus.magic_wand->magic_wand.quality; 
    return self->intel + bonus;
}

int get_crit(Player* self) {
    int bonus = self->bonus.accuracy == NULL ? 0 : self->bonus.accuracy->accuracy.crit;
    return self->crit + bonus;
}

static void attack(Player* self, Monster* monster, int dmg){
    int crit = get_crit(self);
    int random = randrange(0, 100);

    if (random <= crit){
        dmg *= CRIT_MULTIPLICATOR;
    }

    monster->hp -= dmg;
}

void physical_attack(Player* self, Monster* monster){
    attack(self, monster, get_atk(self));
    printf("PHYSICAL\n");
}

void magical_attack(Player* self, Monster* monster){
    assert(self->mp >= 20);

    attack(self, monster, 2 * get_intel(self));
    self->mp -= 20;
    printf("MAGICAL\n");
}

bool attack_monster(Player* self, Monster* monster) {
    switch (self->attackType) {
    case PHYSICAL:
        physical_attack(self, monster);
        return true;
    case MAGICAL:
        if (self->mp < 20) return false;
        magical_attack(self, monster);
        return true;
    }
    return false;
}

void attack_player(Monster* monster, Player* player) {
    int crit = monster->crit;
    int dmg = monster->atk;
    int random = randrange(0, 100);

    if (random <= crit){
        dmg *= CRIT_MULTIPLICATOR;
    }

    player->hp -= dmg;
}

static bool can_move(Map* map, Player* player, int dx, int dy) {
    int x = player->position.x + dx;
    int y = player->position.y + dy;

    if (!is_on_the_grid(x, y)) return false;

    return is_walkable(map, x, y);
}

static void move(Map* map, Player* player, int dx, int dy) {
    player->position.x += dx;
    player->position.y += dy;
}

bool try_move(Player* player, Map* map, int dx, int dy) {
    if (!can_move(map, player, dx, dy)) return false;

    move(map, player, dx, dy);
    return true;
}

void rotate_player(Player* self, double radians) {
    double oldDirX, oldPlaneX;

    oldDirX = self->direction.x;
    self->direction.x = self->direction.x * cos(radians) - self->direction.y * sin(radians);
    self->direction.y = oldDirX * sin(radians) + self->direction.y * cos(radians);

    oldPlaneX = self->plane.x;
    self->plane.x = self->plane.x * cos(-radians) - self->plane.y * sin(radians);
    self->plane.y = oldPlaneX * sin(radians) + self->plane.y * cos(radians);
}

static void update_bonus_accuracy(Player* player){
    if (player->bonus.accuracy != NULL){
        bool consumed = apply_potion_accuracy(player->bonus.accuracy, player);
        if (consumed){
            potion_free(player->bonus.accuracy);
            player->bonus.accuracy = NULL;
        }
    }
}

static void update_bonus_experience(Player* player){
    if (player->bonus.experience != NULL){
        bool consumed = apply_potion_experience(player->bonus.experience, player);
        if (consumed){
            potion_free(player->bonus.experience);
            player->bonus.experience = NULL;
        }
    }
}

static void update_bonus_regeneration(Player* player){
    if (player->bonus.regeneration != NULL){
        bool consumed = apply_potion_regeneration(player->bonus.regeneration, player);
        if (consumed){
            potion_free(player->bonus.regeneration);
            player->bonus.regeneration = NULL;
        }
    }
}

void update_bonus(Player* player){
    update_bonus_accuracy(player);
    update_bonus_experience(player);
    update_bonus_regeneration(player);
}

void auto_equip(Player* player){
    int i;

    for (i = 0; i < NB_ITEMS; i++){
        Item* item = player->inventory.items[i];

        if (item == NULL || item->type != EQUIPMENT) continue;

        Equipment* equipment = item->equipment;

        switch (equipment->type)
        {
        case WEAPON:
            if (player->bonus.weapon == NULL || equipment->weapon.quality >= player->bonus.weapon->weapon.quality){
                player->bonus.weapon = equipment;
            }
            break;

        case ARMOR:
            if (player->bonus.armor == NULL || equipment->armor.quality >= player->bonus.armor->armor.quality){
                player->bonus.armor = equipment;
            }
            break;
        
        case MAGICWAND:
            if (player->bonus.magic_wand == NULL || equipment->magic_wand.quality >= player->bonus.magic_wand->magic_wand.quality){
                player->bonus.magic_wand = equipment;
            }
            break;

        default:
            break;
        }
    }
}

bool verif_equiped(Player* player, Item* item){
    if (item == NULL || item->type != EQUIPMENT) return false;

    Equipment* equipment = item->equipment;

    switch (equipment->type)
    {
        case WEAPON:
            return player->bonus.weapon == equipment;
        
        case ARMOR:
            return player->bonus.armor == equipment;
        
        case MAGICWAND:
            return player->bonus.magic_wand == equipment;

        default:
            return false;
    }
}

int required_experience(int level) {
    return 350 + 50 * level;
}

static void verif_level_up(Player* player) {
    int exp_required = required_experience(player->lvl);

    while (player->exp >= exp_required) {
        player->exp -= exp_required;
        player->lvl++;

        player->hp = get_hp_player(player);
        player->mp = get_mp_player(player);
        player->skill_points += 3;
    }
}

void gain_experience(Player* player, int exp) {
    int bonus = player->bonus.experience == NULL ? 0 : player->bonus.experience->experience.exp;

    player->exp += exp + exp * bonus / 100.0;
    verif_level_up(player);
    printf("Exp: %d lvl: %d \n", player->exp, player->lvl);
}

int get_empty_slot(Player* player){
    int i;

    for(i = 0; i < NB_ITEMS; i++){
        Item* item = player->inventory.items[i];

        if (item == NULL) return i;
    }
    return -1;
}

bool insert_item(Player* player, Item* item, int index){
    if (player->inventory.items[index] != NULL) return false;

    player->inventory.items[index] = item;
    return true;
}