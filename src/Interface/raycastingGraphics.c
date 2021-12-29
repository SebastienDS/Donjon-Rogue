#include <MLV/MLV_all.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#include "Core/GameStates.h"
#include "Interface/View.h"
#include "Util/util.h"
#include "Entity/Player.h"
#include "Map/Map.h"
#include "constants.h"


static void draw_buffer(uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH]) {
    int i, j;
    for (j = 0; j < SCREEN_HEIGHT; j++) {
        for (i = 0; i < SCREEN_WIDTH; i++) {
            MLV_draw_pixel(i, j, buffer[j][i]);
        }
    }
}

void draw_raycasting_graphics(GameStates* gs, View* view) {
    Player* player = get_player(gs);
    Map* map = get_current_map(gs);

    double posX = player->position.x + 0.5;
    double posY = player->position.y + 0.5;

    double dirX = player->direction.x;
    double dirY = player->direction.y;

    double planeX = player->plane.x;
    double planeY = player->plane.y;

    uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] = {0};


    int x, y;

    for (x = 0; x < SCREEN_WIDTH; x++) {
        /* WALL CASTING */

        double cameraX = (double)(x - SCREEN_WIDTH / 2) / (double)(SCREEN_WIDTH / 2);
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;

        int mapX = (int)posX;
        int mapY = (int)posY;

        double sideDistX;
        double sideDistY;

        double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1.0 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1.0 / rayDirY);
        double perpWallDist;

        int stepX;
        int stepY;

        int hit = 0;
        int side;

        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (posX - mapX) * deltaDistX;
        }
        else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - posX) * deltaDistX;
        }
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (posY - mapY) * deltaDistY;
        }
        else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - posY) * deltaDistY;
        }

        while (hit == 0) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

            Celltype cellType = get_cell(map, mapX, mapY)->type;
            if (cellType == WALL || cellType == TREASURE || cellType == MONSTER) hit = 1;
        } 

        if (side == 0) perpWallDist = (sideDistX - deltaDistX);
        else          perpWallDist = (sideDistY - deltaDistY);


        int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

        int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;


        double wallX;
        if (side == 0) wallX = posY + perpWallDist * rayDirY;
        else           wallX = posX + perpWallDist * rayDirX;
        wallX -= floor(wallX);

        int texture_x = (int)(wallX * (double)(TEXTURE_SIZE));
        if (side == 0 && rayDirX > 0) texture_x = TEXTURE_SIZE - texture_x - 1;
        if (side == 1 && rayDirY < 0) texture_x = TEXTURE_SIZE - texture_x - 1;

        double step = 1.0 * TEXTURE_SIZE / lineHeight;
        double texture_pos = (drawStart - SCREEN_HEIGHT / 2.0 + lineHeight / 2.0) * step;

        for (y = drawStart; y < drawEnd; y++) {
            int texture_y = (int)texture_pos & (TEXTURE_SIZE - 1);
            texture_pos += step;

            int r, g, b, a;
            MLV_Color color;

            Cell* cell = get_cell(map, mapX, mapY);

            switch(cell->type) {
                case WALL: 
                    MLV_get_pixel_on_image(view->textures.wall, texture_x, texture_y, &r, &g, &b, &a);
                    color = MLV_convert_rgba_to_color(r, g, b, a);
                    break;
                case MONSTER:
                    MLV_get_pixel_on_image(view->textures.zombie, texture_x, texture_y, &r, &g, &b, &a);
                    color = MLV_convert_rgba_to_color(r, g, b, a);
                    break;
                case TREASURE:
                    if (cell->treasure.state == CLOSE) {
                        MLV_get_pixel_on_image(view->textures.treasure.close, texture_x, texture_y, &r, &g, &b, &a);
                    }
                    else {
                        MLV_get_pixel_on_image(view->textures.treasure.open, texture_x, texture_y, &r, &g, &b, &a);
                    }
                    color = MLV_convert_rgba_to_color(r, g, b, a);
                    break;
                default: color = MLV_COLOR_YELLOW; break;
            }

            if (side == 1) {
                color = MLV_convert_rgba_to_color(r / 2, g / 2, b / 2, a);
            } 
            buffer[y][x] = color;
        }


        /* FLOOR CASTING */

        double floorXWall, floorYWall;

        if (side == 0 && rayDirX > 0) {
            floorXWall = mapX;
            floorYWall = mapY + wallX;
        }
        else if (side == 0 && rayDirX < 0) {
            floorXWall = mapX + 1.0;
            floorYWall = mapY + wallX;
        }
        else if(side == 1 && rayDirY > 0) {
            floorXWall = mapX + wallX;
            floorYWall = mapY;
        }
        else {
            floorXWall = mapX + wallX;
            floorYWall = mapY + 1.0;
        }

        double distWall, distPlayer, currentDist;

        distWall = perpWallDist;
        distPlayer = 0.0;

        if (drawEnd < 0) drawEnd = SCREEN_HEIGHT;

        for(y = drawEnd + 1; y < SCREEN_HEIGHT; y++) {
            currentDist = SCREEN_HEIGHT / (2.0 * y - SCREEN_HEIGHT);

            double weight = (currentDist - distPlayer) / (distWall - distPlayer);

            double currentFloorX = weight * floorXWall + (1.0 - weight) * posX;
            double currentFloorY = weight * floorYWall + (1.0 - weight) * posY;

            int floorTexX, floorTexY;
            floorTexX = (int)(currentFloorX * TEXTURE_SIZE) % TEXTURE_SIZE;
            floorTexY = (int)(currentFloorY * TEXTURE_SIZE) % TEXTURE_SIZE;


            int r, g, b, a;
            MLV_Color color;

            Cell* cell = get_cell(map, (int)currentFloorX, (int)currentFloorY);

            switch(cell->type) {
                case ROOM: 
                    MLV_get_pixel_on_image(view->textures.floor, floorTexX, floorTexY, &r, &g, &b, &a);
                    color = MLV_convert_rgba_to_color(r, g, b, a);
                    break;
                case STAIR_UP:
                case STAIR_DOWN:
                    MLV_get_pixel_on_image(view->textures.stair, floorTexX, floorTexY, &r, &g, &b, &a);
                    color = MLV_convert_rgba_to_color(r, g, b, a);
                    break;
                default: color = MLV_COLOR_BLACK; break;
            }


            buffer[y][x] = color;
            buffer[SCREEN_HEIGHT - y][x] = MLV_rgba(72 / 2, 56 / 2, 58 / 2, 255);
        }
    }

    draw_buffer(buffer);
}