#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <MLV/MLV_all.h>

#include "Core/core.h"
#include "Core/GameStates.h"
#include "Core/Events.h"
#include "Map/Map.h"
#include "Interface/View.h"
#include "constants.h"
#include "Core/save.h"

typedef struct {
    GameStates* gs;
    bool exit;
} Data;

void exit_function(void* data){
    Data* var = (Data*) data;
    var->exit = true;
    save_gamestates(var->gs, "bin/save.txt");
}

int main(void) {
    /* srand(time(NULL)); */
    srand(2);

    GameStates gs;
    Events events;
    View view;

    Data data;
    data.gs = &gs;
    data.exit = false;

    MLV_execute_at_exit( exit_function, &data);

    MLV_create_window("Donjon Rogue", "donjon-rogue", SCREEN_WIDTH, SCREEN_HEIGHT);

    init_view(&view);
    init_game_states(&gs);


    while (!data.exit) {
        events.event = MLV_get_event(&events.key, NULL, NULL, NULL, NULL, &events.mouseX, &events.mouseY, NULL, &events.state);

        update(&gs, &events);
        draw(&gs, &view);
        MLV_actualise_window();
    }

    destroy_view(&view);
    destroy_game_states(&gs);

    MLV_free_window();

    return 0;
}