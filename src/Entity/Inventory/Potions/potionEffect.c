#include "Entity/Inventory/Potions/potionEffect.h"
#include <stdbool.h>
#include <stdlib.h>

bool use_potion_health(Potion* potion, Player* player){
    if (player->hp == get_hp_player(player)) return false;

    player->hp += get_hp_player(player) * potion->health.hp / 100.0;
    player->hp = player->hp > get_hp_player(player) ? get_hp_player(player) : player->hp;
    return true;
}

bool use_potion_accuracy(Potion* potion, Player* player){
    if (player->bonus.accuracy != NULL) return false;

    player->bonus.accuracy = potion;
    return true;
}

bool use_potion_magic(Potion* potion, Player* player){
    if (player->mp == get_mp_player(player)) return false;

    player->mp += get_mp_player(player) * potion->magic.mp / 100.0;
    player->mp = player->mp > get_mp_player(player) ? get_mp_player(player) : player->mp;
    return true;
}

bool use_potion_regeneration(Potion* potion, Player* player){
    if (player->bonus.regeneration != NULL) return false;
    if (player->hp == get_hp_player(player) && player->mp == get_mp_player(player)) return false;

    player->bonus.regeneration = potion;
    return true;
}

bool use_potion_experience(Potion* potion, Player* player){
    if (player->bonus.experience != NULL) return false;

    player->bonus.experience = potion;
    return true;
}

bool use_potion(Potion* potion, Player* player){
    bool used = false;

    switch (potion->type)
    {
    case ACCURACY:
        used = use_potion_accuracy(potion, player);
        apply_potion_accuracy(potion, player);
        break;
    case EXPERIENCE:
        used = use_potion_experience(potion, player);
        apply_potion_experience(potion, player);
        break;
    case HEALTH:
        used = use_potion_health(potion, player);

        if (used) potion_free(potion);
        break;
    case MAGIC:
        used = use_potion_magic(potion, player);

        if (used) potion_free(potion);
        break;
    case REGENERATION:
        used = use_potion_regeneration(potion, player);
        apply_potion_regeneration(potion, player);
        break;
    default:
        break;
    }
    return used;
}

bool apply_potion_regeneration(Potion* potion, Player* player){
    potion->regeneration.countdown--;
    potion->regeneration.duration--;

    if (potion->regeneration.countdown <= 0){
        player->hp += potion->regeneration.hp;
        player->hp = player->hp > get_hp_player(player) ? get_hp_player(player) : player->hp;

        player->mp += potion->regeneration.mp;
        player->mp = player->mp > get_mp_player(player) ? get_mp_player(player) : player->mp;

        potion->regeneration.countdown = potion->regeneration.interval; 
    }

    if (potion->regeneration.duration <= 0) return true;

    return false;
}

bool apply_potion_accuracy(Potion* potion, Player* player){
    potion->accuracy.duration--;
    
    if (potion->accuracy.duration <= 0) return true;

    return false;
}

bool apply_potion_experience(Potion* potion, Player* player){
    potion->experience.duration--;
    
    if (potion->experience.duration <= 0) return true;

    return false;
}