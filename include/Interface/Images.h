#ifndef __IMAGES__
#define __IMAGES__ 

#include <MLV/MLV_all.h>

typedef struct {
    MLV_Image* treasure;
} Images;


/**
 * @brief Initialise les images
 * 
 * @param img 
 */
void init_images(Images* img);

/**
 * @brief Libere la memoire allouee lors de linitialisation
 * 
 * @param img 
 */
void free_images(Images* img);

#endif