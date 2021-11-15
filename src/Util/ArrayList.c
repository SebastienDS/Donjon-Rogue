#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "Util/ArrayList.h"
#include "Util/Element.h"


ArrayList* arrayList_new() {
    ArrayList* list = (ArrayList*)malloc(sizeof(ArrayList));
    if (list == NULL) {
        perror("ArrayList malloc error");
        exit(1);
    }

    list->length = 0;
    list->maxCapacity = 10;
    list->tab = (Element*)malloc(sizeof(Element) * list->maxCapacity);
    if (list->tab == NULL) {
        perror("ArrayList->tab malloc error");
        exit(1);
    }

    return list;
}

void arrayList_free(ArrayList* self, void(*free_func)(Element)) {
    if (free_func != NULL) {
        int i;
        for (i = 0; i < self->length; i++) {
            free_func(self->tab[i]);
        }
    }
    
    free(self->tab);
    self->tab = NULL;
    free(self);
    self = NULL;
}

static void arrayList_realloc(ArrayList* self) {
    self->maxCapacity *= 2;
    self->tab = (Element*)realloc(self->tab, sizeof(Element) * self->maxCapacity);
    if (self->tab == NULL) {
        perror("ArrayList->tab realloc error");
        exit(1);
    }
}

void arrayList_print(ArrayList* self, void(*print_func)(Element)) {
    int i;

    printf("[");
    for (i = 0; i < self->length; i++) {
        print_func(self->tab[i]);
        printf(", ");
    }
    printf("]");
}

void arrayList_add(ArrayList* self, Element value) {
    if (self->length >= self->maxCapacity) {
        arrayList_realloc(self);
    }
    self->tab[self->length] = value;
    self->length++;
}

Element arrayList_remove(ArrayList* self, int index) {
    assert(index < self->length);
    Element value = self->tab[index];

    int i;
    for (i = index; i < self->length - 1; i++) {
        self->tab[i] = self->tab[i + 1];
    }

    self->length--;
    return value;
}

Element arrayList_get(ArrayList* self, int index) {
    assert(index < self->length);
    return self->tab[index];
}

bool arrayList_contains(ArrayList* self, Element elem){
    int i;

    for(i = 0; i < self->length; i++){
        if (self->tab[i] == elem){
            return true;
        }
    }
    return false;
}