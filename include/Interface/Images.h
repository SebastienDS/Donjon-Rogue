#ifndef __IMAGES__
#define __IMAGES__ 

#include <MLV/MLV_all.h>

typedef struct {
    MLV_Image* open;
    MLV_Image* close;
} Treasure_img;


typedef struct {
    Treasure_img treasure;
    MLV_Image* zombie;
    MLV_Image* player;
    MLV_Image* stair;
    MLV_Image* floor;
    MLV_Image* wall;
} Images;

/**
 * @brief Charge les images
 * 
 * @param filename 
 * @param width 
 * @param height 
 * @return MLV_Image* 
 */
MLV_Image* load_image(const char* filename, int width, int height);


/**
 * @brief Initialise les images
 * 
 * @param img 
 */
void init_images(Images* img, int size);

/**
 * @brief Libere la memoire allouee lors de linitialisation
 * 
 * @param img 
 */
void free_images(Images* img);

#endif