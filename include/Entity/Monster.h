#ifndef __MONSTER__
#define __MONSTER__

#include <stdbool.h>

typedef struct {
    int max_hp;
    int hp;
    int atk;
    int crit;
    int exp_given;
} Monster;

/**
 * @brief Initialise un monstre
 * 
 * @param monster 
 */
void init_monster(Monster* monster, int difficulty, int exp_given);

/**
 * @brief Renvoie true si le monstre est mort
 * 
 * @param monster 
 * @return true 
 * @return false 
 */
bool is_monster_dead(Monster* monster);


#endif