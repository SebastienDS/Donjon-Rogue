#include "Core/core.h"
#include "Core/GameStates.h"
#include "Core/Events.h"
#include "Interface/graphics.h"
#include "Interface/interface.h"


void init(GameStates* gs) {
    init_map(&gs->map);
    generate_stage(&gs->map);

    init_player(&gs->player, START_X, START_Y);
}

void update(GameStates* gs, Events* events) {

}

void draw(GameStates* gs) {
    MLV_clear_window(MLV_COLOR_BLACK);
    
    draw_graphics(gs);
    draw_interface(gs);
}