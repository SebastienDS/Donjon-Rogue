#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Map/Map.h"

void print_map(Map* map);

int main() {
    srand(time(NULL));

    Map map;
    init_map(&map);
    generate_stage(&map);

    print_map(&map);

    return 0;
}

void print_map(Map* map) {
    int i, j;

    for (j = 0; j < HEIGHT; j++) {
        for (i = 0; i < WIDTH; i++) {
            if (map->map[j][i].type == WALL) {
                printf(" ");
            } else if (map->map[j][i].type == STAIR_UP) {
                printf("x");
            } else {
                printf("o");
            }
        }
        printf("\n");
    }
}