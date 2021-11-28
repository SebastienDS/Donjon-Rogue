#include "Entity/Player.h"
#include "constants.h"

void init_player(Player* self, int x, int y) {
    self->pos.x = x;
    self->pos.y = y;
}
