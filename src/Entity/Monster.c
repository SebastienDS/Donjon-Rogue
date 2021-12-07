#include "Entity/Monster.h"

void init_monster(Monster* monster){
    monster->hp = 30;
    monster->atk = 10;
}

bool is_dead(Monster* monster){
    return monster->hp <= 0;
}
