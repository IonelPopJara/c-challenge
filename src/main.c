/**
 * I'm so sorry for everything I've done.
 *
 * With love and regret,
 * Tuhkis
 */

#include "raylib.h"
#include "stdio.h" // for printf

#include "app.h"
#include "dyn_array.h"
#include "style.h"
#include "ui.h"
#include "util.h"

#define TOP_BAR_HEIGHT (64)
#define TOP_BAR_PADDING (8)

// THIS IS PUBLICLY ACCESSED. DO NOT ACCESS FROM THREADS IF YOU DON't KNOW WHAT YOU'RE DOING
APP_STATE app = {0};

void draw_top_bar(void) {
    DrawRectangle(0, 0, GetScreenWidth(), TOP_BAR_HEIGHT, ACCENT_COLOR1);

    DrawRectangle(TOP_BAR_PADDING, TOP_BAR_PADDING, 64, TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING, ACCENT_COLOR2);
        DrawRectangle(TOP_BAR_PADDING + 4, TOP_BAR_PADDING + 4, 16, TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING - 8, BG_COLOR1);
        DrawRectangle(TOP_BAR_PADDING + 4 + 16 + 4, TOP_BAR_PADDING + 4, 16, TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING - 8, BG_COLOR1);
        DrawRectangle(TOP_BAR_PADDING + 4 + 16 + 4 + 16 + 4, TOP_BAR_PADDING + 4, 16, TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING - 8, BG_COLOR1);
    if (BUTTON_RELEASED == test_button(TOP_BAR_PADDING, TOP_BAR_PADDING, 64, TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING)) {
        printf("Hello\n");
    }
}

#define BODY_PADDING (12)
#define DATE_OUTLINE (4)
#define DATE_WIDTH (200)
void draw_body(void) {
    DrawRectangle(BODY_PADDING, TOP_BAR_HEIGHT + BODY_PADDING, DATE_WIDTH, GetScreenHeight() - TOP_BAR_HEIGHT - 2 * BODY_PADDING, BORDER_COLOR1);
    DrawRectangle(BODY_PADDING + DATE_OUTLINE, TOP_BAR_HEIGHT + BODY_PADDING + DATE_OUTLINE, DATE_WIDTH - 2 * DATE_OUTLINE, GetScreenHeight() - TOP_BAR_HEIGHT - 2 * BODY_PADDING - 2 * DATE_OUTLINE, BG_COLOR2);
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    InitWindow(800, 550, "Hacktoberfest");

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BG_COLOR1);
            draw_top_bar();
            draw_body();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
