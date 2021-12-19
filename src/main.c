#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "Core/core.h"
#include "Core/GameStates.h"
#include "Core/Events.h"
#include "Map/Map.h"
#include "Interface/Images.h"
#include "constants.h"
#include <MLV/MLV_all.h>



int main(void) {
    /* srand(time(NULL)); */
    srand(2);

    GameStates gs;
    Events events;
    Images images;

    MLV_create_window("Donjon Rogue", "donjon-rogue", SCREEN_WIDTH, SCREEN_HEIGHT);

    MLV_Font* font = MLV_load_font("resources/font/neon_pixel-7.ttf", 50);
    init_images(&images);
    init_game_states(&gs);

    MLV_change_frame_rate(60);

    while (true) {
        events.event = MLV_get_event(&events.key, NULL, NULL, NULL, NULL, &events.mouseX, &events.mouseY, NULL, &events.state);

        update(&gs, &events);
        draw(&gs, &images, font);
        MLV_actualise_window();

        MLV_delay_according_to_frame_rate();
    }

    free_images(&images);
    destroy_game_states(&gs);

    MLV_free_font(font);
    MLV_free_window();

    return 0;
}