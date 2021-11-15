#ifndef __ARRAYLIST__
#define __ARRAYLIST__

#include "Util/Element.h"
#include <stdbool.h>


typedef struct {
    Element* tab;
    int length;
    int maxCapacity;
} ArrayList;

ArrayList* arrayList_new();
void arrayList_free(ArrayList* self, void(*free_func)(Element));
void arrayList_print(ArrayList* self, void(*print_func)(Element));

void arrayList_add(ArrayList* self, Element value);
Element arrayList_remove(ArrayList* self, int index);
Element arrayList_get(ArrayList* self, int index);

/**
 * @brief 
 * 
 * @param self 
 * @param elem 
 * @return true 
 * @return false 
 */
bool arrayList_contains(ArrayList* self, Element elem);


#endif