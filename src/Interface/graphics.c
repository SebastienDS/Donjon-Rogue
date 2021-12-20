#include <MLV/MLV_all.h>
#include "Interface/graphics.h"
#include "Interface/Images.h"
#include "Map/Map.h"
#include "Entity/Player.h"
#include "Core/GameStates.h"
#include "Util/PathFinding/Astar.h"
#include "constants.h"

static void draw_map(Map* map, Player* player, Images* images) {
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
                /* MLV_draw_image(images->room, x * CELL_SIZE - offset_x, y * CELL_SIZE - offset_y); */
            } 
            else if (cell->type == TREASURE) {
                MLV_draw_image(images->floor, x * CELL_SIZE - offset_x, y * CELL_SIZE - offset_y);

                if (cell->treasure.state == CLOSE) {
                    MLV_draw_image(images->treasure.close, x * CELL_SIZE - offset_x, y * CELL_SIZE - offset_y);
                }
                else {
                    MLV_draw_image(images->treasure.open, x * CELL_SIZE - offset_x, y * CELL_SIZE - offset_y);
                }
            } 
            else if (cell->type == MONSTER) {
                MLV_draw_image(images->floor, x * CELL_SIZE - offset_x, y * CELL_SIZE - offset_y);
                MLV_draw_image(images->zombie, x * CELL_SIZE - offset_x, y * CELL_SIZE - offset_y);

            } 
            else if (cell->type == ROOM){
                MLV_draw_image(images->floor, x * CELL_SIZE - offset_x, y * CELL_SIZE - offset_y);
            }
            else if (cell->type == STAIR_UP || cell->type == STAIR_DOWN) {
                MLV_draw_image(images->stair, x * CELL_SIZE - offset_x, y * CELL_SIZE - offset_y);
            }
            else {
                MLV_draw_filled_rectangle(x * CELL_SIZE - offset_x, y * CELL_SIZE - offset_y, CELL_SIZE, CELL_SIZE, MLV_COLOR_DEEP_PINK);
            }
        }
    }
}

static void draw_player(Player* player, Images* images) {
    MLV_draw_image(images->player, (SCREEN_WIDTH - CELL_SIZE) / 2, (SCREEN_HEIGHT - CELL_SIZE) / 2);
}

static void draw_path_to_stair(GameStates* gs) {
    Player* player = get_player(gs);

    int p_x = player->pos.x;
    int p_y = player->pos.y;

    int width = VISION_X / 2;
    int height = VISION_Y / 2;

    int offset_x = CELL_SIZE * (p_x - width);
    int offset_y = CELL_SIZE * (p_y - height);

    Node* node;
    for (node = gs->path_to_stair->node; node != NULL; node = node->next) {
        PathPosition* position = (PathPosition*)node->value;
        int x = position->x * CELL_SIZE - offset_x;
        int y = position->y * CELL_SIZE - offset_y;

        MLV_draw_rectangle(x, y, CELL_SIZE, CELL_SIZE, MLV_COLOR_DEEP_PINK);
    }
}

void draw_graphics(GameStates* gs, Images* images) {
    draw_map(get_current_map(gs), get_player(gs), images);

    #if DEBUG
        if (gs->path_to_stair != NULL) draw_path_to_stair(gs);
    #endif
    draw_player(get_player(gs), images);
}