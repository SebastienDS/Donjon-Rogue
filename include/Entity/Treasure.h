#ifndef __TREASURE__
#define __TREASURE__

typedef enum {
        OPEN, CLOSE
} State;

typedef struct {
    State state;
} Treasure;


#endif