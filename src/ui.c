#include "raylib.h"

#include "ui.h"

BUTTON_STATE test_button(int x, int y, int w, int h) {
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
    return CheckCollisionPointRec(GetMousePosition(), (Rectangle) {x, y, w, h}) ? (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) ? BUTTON_RELEASED : (IsMouseButtonDown(MOUSE_BUTTON_LEFT) ? BUTTON_PRESSED : BUTTON_HOVER)) : BUTTON_NORMAL;
}

int process_text_input(int focused, char* buffer, int writeable_length, int x, int y, int font_size) {
  if (focused) {
  }
  else {
    
  }
  return 0;
}

