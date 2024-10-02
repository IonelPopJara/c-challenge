/**
 * Simple (and kinda cumbersome) gui widget handler for this project.
 * This "module" won't render anything. Only deals with the logic.
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

BUTTON_STATE test_button(int x, int y, int w, int h);

// NOTE(Tuhkis): I prefer `int` over `_Bool` because `_Bool` introduces secret extra behviour with some compileres.
int process_text_input(int focused, char* buffer, int writeable_length, int x, int y, int font_size);

#endif // UI_H

