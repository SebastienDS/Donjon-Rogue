#include <MLV/MLV_all.h>
#include "Interface/graphics.h"
#include "Map/Map.h"
#include "Entity/Player.h"
#include "Core/GameStates.h"
#include "constants.h"

static void draw_map(Map* map, Player* player) {
    int p_x = player->pos.x;
    int p_y = player->pos.y;

    int width = VISION_X / 2;
    int height = VISION_Y / 2;

    int offset_x = CELL_SIZE * (p_x - width);
    int offset_y = CELL_SIZE * (p_y - height);

    int i, j;

    for (j = -height; j <= height; j++) {
        for (i = -width; i <= width; i++) {
            int x = p_x + i;
            int y = p_y + j;

            if (!is_on_the_grid(x, y)) {
                continue;
            }

            Cell* cell = get_cell(map, x, y);

            if (cell->type == WALL) {
                MLV_draw_filled_rectangle(x * CELL_SIZE - offset_x, y * CELL_SIZE - offset_y, CELL_SIZE, CELL_SIZE, MLV_COLOR_BLACK);
            } 
            else if (cell->type == TREASURE) {
                MLV_draw_filled_rectangle(x * CELL_SIZE - offset_x, y * CELL_SIZE - offset_y, CELL_SIZE, CELL_SIZE, MLV_COLOR_YELLOW);
            } 
            else if (cell->type == MONSTER) {
                MLV_draw_filled_rectangle(x * CELL_SIZE - offset_x, y * CELL_SIZE - offset_y, CELL_SIZE, CELL_SIZE, MLV_COLOR_RED);
            } 
            else {
                MLV_draw_filled_rectangle(x * CELL_SIZE - offset_x, y * CELL_SIZE - offset_y, CELL_SIZE, CELL_SIZE, MLV_COLOR_WHITE);
            }
        }
    }
}

static void draw_player(Player* player) {
    MLV_draw_filled_rectangle((SCREEN_WIDTH - CELL_SIZE) / 2, (SCREEN_HEIGHT - CELL_SIZE) / 2, CELL_SIZE, CELL_SIZE, MLV_COLOR_BLUE);
}

void draw_graphics(GameStates* gs) {
    draw_map(&gs->map, &gs->player);
    draw_player(&gs->player);
}