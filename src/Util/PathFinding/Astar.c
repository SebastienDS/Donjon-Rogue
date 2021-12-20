#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "Util/PathFinding/Astar.h"
#include "Util/ArrayList.h"
#include "Util/LinkedList.h"
#include "Util/Element.h"


#define NEIGHBOURS_COUNT 4


typedef struct AstarNode {
    struct AstarNode* parent;
    struct PathPosition* position;
    int g;
    int h;
    int f;
} AstarNode;


bool position_equals(PathPosition* pos, PathPosition* pos2) {
    return pos->x == pos2->x && pos->y == pos2->y;
}

PathPosition* path_position_new(int x, int y) {
    PathPosition* pos = (PathPosition*)malloc(sizeof(PathPosition));
    if (pos == NULL) {
        perror("Position malloc error");
        exit(1);
    }

    pos->x = x;
    pos->y = y;
    pos->isPartOfFinalPath = false;

    return pos;
}

void path_position_free(PathPosition* self) {
    if (self != NULL) free(self);
    self = NULL;
}

AstarNode* astarNode_new(AstarNode* parent, PathPosition* position) {
    AstarNode* node = (AstarNode*)malloc(sizeof(AstarNode));
    if (node == NULL) {
        perror("AstarNode malloc error");
        exit(1);
    }

    node->parent = parent;
    node->position = position;

    node->g = 0;
    node->h = 0;
    node->f = 0;

    return node;
}

void astarNode_free(AstarNode* self) {
    free(self);
    self = NULL;
}

void free_path_position_element(Element element) {
    PathPosition* pos = (PathPosition*)element;
    if (pos != NULL) path_position_free(pos);
    pos = NULL;
}

void print_path_position_element(Element element) {
    PathPosition* pos = (PathPosition*)element;
    printf("(%d, %d)", pos->x, pos->y);
}

bool is_already_in_closed_list(ArrayList* closed_list, AstarNode* child) {
    int i;
    for (i = 0; i < closed_list->length; i++) {
        AstarNode* closed_child = arrayList_get(closed_list, i);

        if (position_equals(child->position, closed_child->position)) {
            return true;
        }
    }
    return false;
}

bool is_already_in_open_list(ArrayList* open_list, AstarNode* child) {
    int i;
    for (i = 0; i < open_list->length; i++) {
        AstarNode* open_node = arrayList_get(open_list, i);

        if (position_equals(child->position, open_node->position) && child->g > open_node->g) {
            return true;
        }
    }
    return false;
}

LinkedList* fetch_path(AstarNode* current_node) {
    LinkedList* path = linkedList_new();
            
    AstarNode* current = current_node;

    while (current != NULL) {
        current->position->isPartOfFinalPath = true;
        linkedList_push(path, current->position);
        current = current->parent;
    }

    return path;
}

void free_astar_node(Element element) {
    AstarNode* node = (AstarNode*)element;

    if (!node->position->isPartOfFinalPath) path_position_free(node->position);
    astarNode_free(node);
}

LinkedList* astar(Element grid, PathPosition* start, PathPosition* end, bool (*is_walkable_func)(Element* grid, int i, int j), int width, int height) {
    LinkedList* path = NULL;

    AstarNode* start_node = astarNode_new(NULL, start);
    AstarNode* end_node = astarNode_new(NULL, end);

    ArrayList* open_list = arrayList_new();
    ArrayList* closed_list = arrayList_new();

    arrayList_add(open_list, start_node);

    while (open_list->length > 0) {
        AstarNode* current_node = (AstarNode*)arrayList_get(open_list, 0);
        int current_index = 0;

        int i;
        for (i = 0; i < open_list->length; i++) {
            AstarNode* item = (AstarNode*)arrayList_get(open_list, i);
            if (item->f < current_node->f) {
                current_node = item;
                current_index = i;
            }
        }

        arrayList_remove(open_list, current_index);
        arrayList_add(closed_list, current_node);

        if (position_equals(current_node->position, end_node->position)) {
            path = fetch_path(current_node);
            
            break;
        }

        ArrayList* children = arrayList_new();
        int neighbours[][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

        for (i = 0; i < NEIGHBOURS_COUNT; i++) {
            int* position = neighbours[i];
            PathPosition* node_position = path_position_new(current_node->position->x + position[0], current_node->position->y + position[1]);

            if (node_position->x > width - 1 || node_position->x < 0 || node_position->y > height - 1 || node_position->y < 0) {
                path_position_free(node_position);
                continue;
            }

            if (!is_walkable_func(grid, node_position->x, node_position->y)) {
                path_position_free(node_position);
                continue;
            }

            AstarNode* new_node = astarNode_new(current_node, node_position);
            arrayList_add(children, new_node);
        }

        for (i = 0; i < children->length; i++) {
            AstarNode* child = (AstarNode*)arrayList_get(children, i);

            if (is_already_in_closed_list(closed_list, child)) {
                path_position_free(child->position);
                astarNode_free(child);
                continue;
            }

            child->g = current_node->g + 1;
            child->h = pow(child->position->x - end_node->position->x, 2) + pow(child->position->y - end_node->position->y, 2);
            child->f = child->g + child->h;

            if (is_already_in_open_list(open_list, child)) {
                path_position_free(child->position);
                astarNode_free(child);
                continue;
            }

            arrayList_add(open_list, child);
        }

        arrayList_free(children, NULL);
    }

    arrayList_free(open_list, free_astar_node);
    arrayList_free(closed_list, free_astar_node);
    free_astar_node(end_node);

    return path;
}