#include "Entity/Monster.h"
#include "Util/random.h"
#include "Util/util.h"


void init_monster(Monster* monster, int difficulty, int exp_given) {
    monster->max_hp = 30 + difficulty * 2;
    monster->hp = monster->max_hp;
    monster->atk = 5 + difficulty;
    monster->crit = 5;
    monster->exp_given = exp_given * 10; 
}

bool is_dead(Monster* monster) {
    return monster->hp <= 0;
}