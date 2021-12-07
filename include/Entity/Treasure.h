#ifndef __TREASURE__
#define __TREASURE__

typedef enum {
    OPEN, CLOSE
} State;

typedef struct {
    State state;
} Treasure;

/**
 * @brief Initialise un trésor
 * 
 * @param treasure 
 */
void init_treasure(Treasure* treasure);


#endif