#include "Entity/Monster.h"
#include "Entity/Player.h"

void init_monster(Monster* monster){
    monster->max_hp = 30;
    monster->hp = monster->max_hp;
    monster->atk = 5;
    monster->crit = 5;
}

bool is_dead(Monster* monster){
    return monster->hp <= 0;
}