#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "Entity/Player.h"
#include "Entity/Monster.h"
#include "Entity/Inventory/Inventory.h"
#include "Map/Map.h"
#include "Util/random.h"
#include "constants.h"


#define CRIT_MULTIPLICATOR 3

void init_player(Player* self, int x, int y) {
    int i;

    self->position.x = x;
    self->position.y = y;

    self->direction.x = -1;
    self->direction.y = 0;

    self->plane.x = 0;
    self->plane.y = -0.66;
    rotate_player(self, M_PI / 2.0);

    self->atk = 10;
    self->intel = 10;
    self->def = 10;
    self->crit = 5;
    self->lvl = 1;
    self->exp = 0;
    self->hp = get_hp_player(self); 
    self->mp = get_mp_player(self);

    self->bonus.potions[REGENERATION_INDEX] = NULL;
    self->bonus.potions[ACCURACY_INDEX] = NULL;
    self->bonus.potions[EXPERIENCE_INDEX] = NULL;

    init_inventory(&self->inventory);
    for (i = 0; i < 12; i++){
        self->inventory.items[i] = get_random_item(1);
    }
}

int get_hp_player(Player* self){
    return 10 * self->def;
}

int get_mp_player(Player* self){
    return 10 * self->intel - 50;
}

static int get_crit(Player* self){
    if (self->bonus.potions[ACCURACY] == NULL){
        return self->crit;
    }
    assert(self->bonus.potions[ACCURACY]->type == ACCURACY);
    return self->bonus.potions[ACCURACY]->accuracy.crit;
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
    attack(self, monster, self->atk);
}

void magical_attack(Player* self, Monster* monster){
    assert(self->mp >= 20);

    attack(self, monster, 2 * self->intel);
    self->mp -= 20;
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