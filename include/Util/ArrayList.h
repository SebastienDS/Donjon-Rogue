#ifndef __ARRAYLIST__
#define __ARRAYLIST__

#include "Util/Element.h"
#include <stdbool.h>


typedef struct {
    Element* tab;
    int length;
    int maxCapacity;
} ArrayList;

/**
 * @brief Crée une nouvelle ArrayList
 * 
 * @return ArrayList* 
 */
ArrayList* arrayList_new();

/**
 * @brief Libère la mémoire alloué pour l'ArrayList
 * 
 * @param self 
 * @param free_func Fonction permettant la libération du contenu de la liste. 
 *                  NULL si l'on ne souhaite pas libérer la mémoire des éléments
 */
void arrayList_free(ArrayList* self, void(*free_func)(Element));

/**
 * @brief Affiche le contenu de l'ArrayList
 * 
 * @param self 
 * @param print_func Fonction permettant l'affichage de l'Element
 */
void arrayList_print(ArrayList* self, void(*print_func)(Element));

/**
 * @brief Ajoute un élément dans l'ArrayList
 * 
 * @param self 
 * @param value 
 */
void arrayList_add(ArrayList* self, Element value);

/**
 * @brief Retire et renvoie l'élément dans l'ArrayList à l'index souhaité
 * 
 * @param self 
 * @param index 
 * @return Element 
 */
Element arrayList_remove(ArrayList* self, int index);

/**
 * @brief Renvoie l'élément de l'ArrayList à l'index souhaité
 * 
 * @param self 
 * @param index 
 * @return Element 
 */
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