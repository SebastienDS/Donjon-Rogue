#include <stdlib.h>
#include <stdio.h>
#include "Util/LinkedList.h"
#include "Util/Element.h"


Node* node_new(Element value) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        perror("Node malloc error");
        exit(1);
    }

    node->value = value;
    node->next = NULL;

    return node;
}

void node_free(Node* self, void(*free_func)(Element)) {
    if (free_func != NULL && self->value != NULL) free_func(self->value);
    free(self);
}

LinkedList* linkedList_new() {
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    if (list == NULL) {
        perror("LinkedList malloc error");
        exit(1);
    }

    list->length = 0;
    list->node = NULL;

    return list;
}

void linkedList_free(LinkedList* self, void(*free_func)(Element)) {
    linkedList_clear(self, free_func);
    free(self);
}

void linkedList_print(LinkedList* self, void(*print_func)(Element)) {
    Node* node;
    for (node = self->node; node != NULL; node = node->next) {
        print_func(node->value);        
        printf(" -> ");
    }
    printf("NULL\n");
}

void linkedList_push(LinkedList* self, Element value) {
    Node* node = node_new(value);
    self->length++;

    if (self->length == 0) {
        self->node = node;
        return;
    }

    node->next = self->node;
    self->node = node;
}

Element linkedList_pop(LinkedList* self) {
    if (self->length == 0) {
        return NULL;
    }

    Node* node = self->node;
    Element value = node->value;

    self->node = self->node->next;
    self->length--;
    node_free(node, NULL);

    return value;
}

Element linkedList_peek(LinkedList* self) {
    if (self->length == 0) {
        return NULL;
    }
    return self->node->value;
}

void linkedList_clear(LinkedList* self, void(*free_func)(Element)) {
    while (self->length != 0) {
        Element value = linkedList_pop(self);
        if (free_func != NULL && value != NULL) free_func(value);
    }
}