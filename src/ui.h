/**
 * Simple (and kinda cumbersome) IMGUI handler for this project
 */

#ifndef UI_H
#define UI_H

#include "dyn_array.h"

typedef enum button_state {
    BUTTON_NORMAL = 0,
    BUTTON_HOVER,
    BUTTON_PRESSED,
    BUTTON_RELEASED
} BUTTON_STATE;

// NOTE(Tuhkis): I prefer `int` over `_Bool` because `_Bool` introduces secret extra behviour with some compileres.
BUTTON_STATE test_button(int x, int y, int w, int h);

#endif // UI_H

