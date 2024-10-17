#include "raylib.h"
#include "spooky_style.h"

#include "ui.h"

BUTTON_STATE test_button(int x, int y, int w, int h,MouseButton mouse_button) {
    /*
     * Cursed one-liner of ternary incoming.
     * Boils down to:
     * if mouse is over button:
     *     if mouse is pressed:
     *         return button is being held down
     *     else if mouse is released:
     *         return button has been pressed
     *     else:
     *         return button is being hovered
     * else:
     *     return button is not being interacted with
     */
    return CheckCollisionPointRec(GetMousePosition(), (Rectangle) {x, y, w, h}) ? (IsMouseButtonReleased(mouse_button) ? BUTTON_RELEASED : (IsMouseButtonDown(mouse_button) ? BUTTON_PRESSED : BUTTON_HOVER)) : BUTTON_NORMAL;
}

/**
 * I read that there was a way of achieving overloading in C, but I'm too lazy for that so here's
 * the same function as a bove with a slightly different name.
 *
 * - Mults
 */
BUTTON_STATE test_button_struct(BUTTON button, MouseButton mouse_button) {
    // I just copied Tuhkis' cursed one-liner
    return CheckCollisionPointRec(GetMousePosition(), (Rectangle) {button.x, button.y, button.size, button.size}) ? (IsMouseButtonReleased(mouse_button) ? BUTTON_RELEASED : (IsMouseButtonDown(mouse_button) ? BUTTON_PRESSED : BUTTON_HOVER)) : BUTTON_NORMAL;
}

int process_text_input(int focused, char* buffer, int writeable_length, int x, int y, int font_size) {
    if (focused) {
    }
    else {

    }
    return 0;
}

int is_any_key_pressed() {
    int p_key = GetKeyPressed();
    return (p_key >= 32 && p_key <= 125) ? p_key : 0;
}

/**
 * This helper function draws a close button at the top right corner of a
 * "window" (the window is a local window, not the main raylib window, don't
 * get me started). This function receives a pointer to a button so you can
 * use the handle outside of the scope of this function to monitor events.
 */
void draw_close_button(BUTTON* button, int size, WINDOW local_window) {
    button->size = size;
    button->x = local_window.x + local_window.width - button->size - 10;
    button->y = local_window.y + 10;
    DrawRectangle(button->x, button->y, button->size, button->size, BG_COLOR1);
    DrawLine(button->x, button->y, button->x + button->size, button->y + button->size, ACCENT_COLOR2);
    DrawLine(button->x + button->size, button->y, button->x, button->y + button->size, ACCENT_COLOR2);
}

/**
 * This helper function draws a checkbox with the text included.
 * The parameter `state` determines whether the checkbox is
 * rendered as checked or not.
 *
 * The text and checkbox will always be centered in the X axis,
 * and centered in the Y axis by default.
 * You can use y_offset to change the position of the text to fit
 * your design.
 */
void draw_checkbox(BUTTON* button, int size, WINDOW local_window, int y_offset, const char* text, bool state) {
    int distance_between_text_and_checkbox = 10;

    int text_width = MeasureText(text, size);
    int total_width = text_width + distance_between_text_and_checkbox + size;

    int text_x = local_window.x + (local_window.width - total_width) / 2;
    /*int text_y = local_window.y + 30 + y_offset; // Adjust vertical position as needed*/
    int text_y = local_window.y + (local_window.height / 2) + y_offset; // Adjust vertical position as needed
    DrawText(text, text_x, text_y, size, BG_COLOR1);

    button->size = size;
    button->x = text_x + text_width + distance_between_text_and_checkbox;
    button->y = text_y + (size - button->size) / 2; // Center checkbox vertically with text
    DrawRectangle(button->x, button->y, button->size, button->size, BG_COLOR1);

    if (state) {
        DrawLine(button->x, button->y, button->x + button->size, button->y + button->size, ACCENT_COLOR2);
        DrawLine(button->x + button->size, button->y, button->x, button->y + button->size, ACCENT_COLOR2);
    }
}

