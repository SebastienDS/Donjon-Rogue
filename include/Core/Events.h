#ifndef __EVENTS__
#define __EVENTS__

#include <MLV/MLV_all.h>

typedef struct {
    MLV_Event event;
    MLV_Keyboard_button key;
    MLV_Button_state state;
    int mouseX;
    int mouseY;
} Events;

#endif