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
#include "Interface/interface.h"
#include "constants.h"
#include "Core/Backup/load.h"
#include "Core/Backup/save.h"

#define SAVE_PATH "bin/save.txt"

typedef struct {
    GameStates* gs;
    bool exit;
    bool in_game;
    bool game_started;
} Data;

void exit_function(void* data){
    Data* var = (Data*) data;
    var->exit = true;
    if (var->in_game) {
        save_gamestates(var->gs, SAVE_PATH);
    }
}

bool menu(View* view, Data* data) {
    MLV_clear_window(MLV_COLOR_GRAY);

    Button new_game;
    Button reload_game;
    Events events;

    set_button(&new_game, SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 3, 400, 100, "Nouvelle Partie", NULL);
    set_button(&reload_game, SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT * 2 / 3, 400, 100, "Continuer", NULL);


    draw_button(&new_game, view->font);
    draw_button(&reload_game, view->font);


    MLV_actualise_window();

    while (!data->exit) {
        events.event = MLV_get_event(&events.key, NULL, NULL, NULL, NULL, &events.mouseX, &events.mouseY, NULL, &events.state);

        if (!(events.event == MLV_MOUSE_BUTTON && events.state == MLV_PRESSED)) continue;


        if (test_click(&new_game, events.mouseX, events.mouseY)) {
            return true;
        } 
        else if (test_click(&reload_game, events.mouseX, events.mouseY)) {
            return false;
        }
    }
    return false;
}

void game(GameStates* gs, View* view, Data* data, bool new_game) {
    data->in_game = true;

    if (new_game) {
        init_game_states(gs);
    } else {
        load_gamestates(SAVE_PATH, gs);
    }

    data->game_started = true;

    Events events;
    Player* player = get_player(gs);

    while (!data->exit && !is_player_dead(player)) {
        events.event = MLV_get_event(&events.key, NULL, NULL, NULL, NULL, &events.mouseX, &events.mouseY, NULL, &events.state);

        update(gs, &events);
        draw(gs, view);
        MLV_actualise_window();
    }

    data->in_game = false;
}

bool stat(GameStates* gs, View* view, Data* data) {
    remove(SAVE_PATH); /* delete save */

    MLV_clear_window(MLV_COLOR_GRAY);

    char str[255];
    Button play_again;
    Button quit;
    Events events;

    Player* player = get_player(gs);
  
    sprintf(str, "Fin de partie\n\nEtage : %d\nNiveau: %d\nHP Max: %d\nMP Max: %d\nAttaque: %d\nIntel: %d\nDef: %d\n", 
        gs->current_stage, player->lvl, get_hp_player(player), get_mp_player(player), get_atk(player), get_intel(player), get_def(player));
    set_button(&play_again, SCREEN_WIDTH / 3 - 150, SCREEN_HEIGHT - 200, 300, 100, "Play Again", NULL);
    set_button(&quit, SCREEN_WIDTH * 2 / 3 - 150, SCREEN_HEIGHT - 200, 300, 100, "Quit", NULL);


    MLV_draw_text_box_with_font(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT - 100, str, view->font, 15, MLV_COLOR_GRAY, MLV_COLOR_BLACK, MLV_COLOR_GRAY, MLV_TEXT_LEFT, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);

    draw_button(&play_again, view->font);
    draw_button(&quit, view->font);


    MLV_actualise_window();

    while (!data->exit) {
        events.event = MLV_get_event(&events.key, NULL, NULL, NULL, NULL, &events.mouseX, &events.mouseY, NULL, &events.state);

        if (!(events.event == MLV_MOUSE_BUTTON && events.state == MLV_PRESSED)) continue;


        if (test_click(&play_again, events.mouseX, events.mouseY)) {
            return true;
        } 
        else if (test_click(&quit, events.mouseX, events.mouseY)) {
            return false;
        }
    }
    return false;
}

void gameloop(GameStates* gs, View* view, Data* data) {
    bool play = true;
    while (!data->exit && play) {
        bool new_game = menu(view, data);
        if (data->exit) return;

        game(gs, view, data, new_game);
        if (data->exit) return;


        play = stat(gs, view, data);
    }
}

int main(void) {
    srand(time(NULL));

    GameStates gs;
    View view;

    Data data;
    data.gs = &gs;
    data.in_game = false;
    data.exit = false;
    data.game_started = false;


    MLV_execute_at_exit( exit_function, &data);

    MLV_create_window("Donjon Rogue", "donjon-rogue", SCREEN_WIDTH, SCREEN_HEIGHT);

    init_view(&view);

    gameloop(&gs, &view, &data);

    destroy_view(&view);

    if (data.game_started) {
        destroy_game_states(&gs);
    }

    MLV_free_window();

    return 0;
}