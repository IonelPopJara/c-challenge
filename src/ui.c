#include "raylib.h"

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

