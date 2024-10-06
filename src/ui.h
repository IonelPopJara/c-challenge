/**
 * Simple (and kinda cumbersome) gui widget handler for this project.
 * This "module" won't render anything. Only deals with the logic.
 */

#ifndef UI_H
#define UI_H

#include "dyn_array.h"
#include "raylib.h"

typedef enum button_state {
    BUTTON_NORMAL = 0,
    BUTTON_HOVER,
    BUTTON_PRESSED,
    BUTTON_RELEASED
} BUTTON_STATE;

BUTTON_STATE test_button(int x, int y, int w, int h, MouseButton mouse_button);

// NOTE(Tuhkis): I prefer `int` over `_Bool` because `_Bool` introduces secret extra behviour with some compileres.
int process_text_input(int focused, char* buffer, int writeable_length, int x, int y, int font_size);

// We Check if any key in range [32..125] is pressed
int is_any_key_pressed();

#endif // UI_H

