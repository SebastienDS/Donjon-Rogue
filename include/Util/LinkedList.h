#ifndef __LINKEDLIST__
#define __LINKEDLIST__

#include "Util/Element.h"


typedef struct Node {
    Element value;
    struct Node* next;
} Node;

typedef struct {
    Node* node;
    int length;
} LinkedList;

Node* node_new(Element value);
void node_free(Node* self, void(*free_func)(Element));

LinkedList* linkedList_new();
void linkedList_free(LinkedList* self, void(*free_func)(Element));
void linkedList_print(LinkedList* self, void(*print_func)(Element));
void linkedList_push(LinkedList* self, Element value);
Element linkedList_pop(LinkedList* self);
Element linkedList_peek(LinkedList* self);
void linkedList_clear(LinkedList* self, void(*free_func)(Element));


#endif