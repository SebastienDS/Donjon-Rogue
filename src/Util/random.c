#include <stdlib.h>
#include "Util/random.h"

int randrange(int min, int max) {
    return rand() % (max + 1 - min) + min;
}