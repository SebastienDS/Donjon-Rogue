#include "Util/random.h"
#include <stdlib.h>

int randrange(int min, int max){
    return rand() % (max + 1 - min) + min;
}