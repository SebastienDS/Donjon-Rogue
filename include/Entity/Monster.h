#ifndef __MONSTER__
#define __MONSTER__
#include <stdbool.h>

typedef struct {
    int hp;
    int atk;
} Monster;

/**
 * @brief Initialise un monstre
 * 
 * @param monster 
 */
void init_monster(Monster* monster);

/**
 * @brief Renvoie true si le monstre est mort
 * 
 * @param monster 
 * @return true 
 * @return false 
 */
bool is_dead(Monster* monster);

#endif