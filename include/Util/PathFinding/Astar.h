#ifndef __ASTAR__
#define __ASTAR__

#include <stdbool.h>
#include "Util/LinkedList.h"
#include "Util/Element.h"


typedef struct PathPosition {
    int x;
    int y;
    bool isPartOfFinalPath;
} PathPosition;


PathPosition* path_position_new(int x, int y);
void path_position_free(PathPosition* self);

void print_path_position_element(Element element);
void free_path_position_element(Element element);


LinkedList* astar(Element grid, PathPosition* start, PathPosition* end, bool (*is_walkable_func)(Element* grid, int i, int j), int width, int height);


#endif