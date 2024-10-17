/**
 * Simple (and kinda cumbersome) gui widget handler for this project.
 * This "module" won't render anything. Only deals with the logic.
 */

#ifndef UI_H
#define UI_H

#include "dyn_array.h"
#include "raylib.h"

/**
* NOTE: This is different from the main Window created by raylib
* You can think of it more like, bounds where you tell the program
* where to render. Or a local window (idk if this is a term or not)
*
* That's certainly not confusing at all - Mults
*
* Maybe I shouldn't have called it window...
* well, too late :)
*/
typedef struct window {
    int x;
    int y;
    int width;
    int height;
} WINDOW;

typedef struct button {
    int x;
    int y;
    int size;
} BUTTON;

typedef enum button_state {
    BUTTON_NORMAL = 0,
    BUTTON_HOVER,
    BUTTON_PRESSED,
    BUTTON_RELEASED
} BUTTON_STATE;

BUTTON_STATE test_button(int x, int y, int w, int h, MouseButton mouse_button);

BUTTON_STATE test_button_struct(BUTTON button, MouseButton mouse_button);

// NOTE(Tuhkis): I prefer `int` over `_Bool` because `_Bool` introduces secret extra behviour with some compileres.
int process_text_input(int focused, char* buffer, int writeable_length, int x, int y, int font_size);

// We Check if any key in range [32..125] is pressed
int is_any_key_pressed();

// Draw a close button, (the typical x in the top left corner of the screen)
void draw_close_button(BUTTON* button, int size, WINDOW local_window);

// Draw a checkbox with text
void draw_checkbox(BUTTON* button, int size, WINDOW local_window, int y_offset, const char* text, bool state);

#endif // UI_H

