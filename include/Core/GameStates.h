#ifndef __GAMESTATES__
#define __GAMESTATES__

#include "Map/Map.h"
#include "Entity/Player.h"

typedef struct {
    Map map;
    Player player;
} GameStates;

#endif