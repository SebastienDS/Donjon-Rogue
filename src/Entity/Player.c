#include "Entity/Player.h"
#include "Entity/Monster.h"
#include "Entity/Inventory/Inventory.h"
#include "constants.h"
#include <assert.h>
#include <stdlib.h>
#include "Util/random.h"

#define CRIT_MULTIPLICATOR 3

void init_player(Player* self, int x, int y) {
    int i;

    self->pos.x = x;
    self->pos.y = y;
    self->bonus.potions[REGENERATION_INDEX] = NULL;
    self->bonus.potions[ACCURACY_INDEX] = NULL;
    self->bonus.potions[EXPERIENCE_INDEX] = NULL;
    self->atk = 10;
    self->intel = 10;
    self->def = 10;
    self->crit = 5;
    self->lvl = 1;
    self->exp = 0;
    self->hp = get_hp_player(self); 
    self->mp = get_mp_player(self);

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
