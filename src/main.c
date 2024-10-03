/**
 * I'm so sorry for everything I've done.
 *
 * With love and regret,
 * Tuhkis
 */

// Please don't mind the spaghetti in the code, I'm italian - M37

#include "raylib.h"
#include "stdio.h" // for printf (and now also snprintf i guess :P)

#include "app.h"
#include "day.h"
#include "dyn_array.h"
#include "style.h"
#include "ui.h"
#include "util.h"

#define TOP_BAR_HEIGHT (66)
#define TOP_BAR_PADDING (8)
#define SELECT_VIEW_WIDTH (64)
#define SELECT_VIEW_ICON_COLUMNS (3)
#define SELECT_VIEW_ICON_PADDING_OUT (4)
#define SELECT_VIEW_ICON_PADDING_IN (4)

// THIS IS PUBLICLY ACCESSED. DO NOT ACCESS FROM THREADS IF YOU DON't KNOW WHAT YOU'RE DOING
APP_STATE app = {0};

void draw_top_bar(int rows) {
    DrawRectangle(0, 0, GetScreenWidth(), TOP_BAR_HEIGHT, ACCENT_COLOR1);

    DrawRectangle(TOP_BAR_PADDING, TOP_BAR_PADDING, SELECT_VIEW_WIDTH, TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING, ACCENT_COLOR2);
    if (BUTTON_RELEASED == test_button(TOP_BAR_PADDING, TOP_BAR_PADDING, SELECT_VIEW_WIDTH, TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING)) {
        app.view_type = (app.view_type == WEEK_VIEW) ? MONTH_VIEW : WEEK_VIEW; //printf("Switch to week view\n");
    }
    
    int bar_width = (SELECT_VIEW_WIDTH - 2 * SELECT_VIEW_ICON_PADDING_OUT - (SELECT_VIEW_ICON_COLUMNS - 1) * SELECT_VIEW_ICON_PADDING_IN) / SELECT_VIEW_ICON_COLUMNS;
    int bar_height = (TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING - 2 * SELECT_VIEW_ICON_PADDING_OUT - (rows - 1) * SELECT_VIEW_ICON_PADDING_IN) / rows;
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < SELECT_VIEW_ICON_COLUMNS; ++x) {
            DrawRectangle(
                TOP_BAR_PADDING + SELECT_VIEW_ICON_PADDING_OUT + (bar_width + SELECT_VIEW_ICON_PADDING_IN) * x,
                TOP_BAR_PADDING + SELECT_VIEW_ICON_PADDING_OUT + (bar_height + SELECT_VIEW_ICON_PADDING_IN) * y, 
                bar_width, bar_height, BG_COLOR1);
        }
    }
}

// This piece of code is so cursed. Sorry for what i've created - M37
int find_ideal_text_size(char *text, int max_width) {
    int text_size = 32;
    while (text_size > 1 && MeasureText(text, text_size) > max_width) {
        text_size = text_size >> 1;
    }
    for (int s = (32 - text_size) >> 1; s > 0; s = s >> 1) {
        if (MeasureText(text, text_size + s) <= max_width) {
            text_size += s;
        }
    }
    return text_size;
}

// The W and the 8 are (usually) the widest characters among letters and numbers
#define TEST_DATE_STR ("WWW 88ww")
// This should have been declared inside of the "day" file, but idk how to make it visible here if i do that
const char * WEEKDAY_NAMES[] = {
    "Monday", "Tuesday", 
    "Wednesday", "Thursday",
    "Friday", "Saturday", "Sunday"
};

#define BODY_PADDING_OUT (8)
#define BODY_PADDING_IN (2)
#define DATE_OUTLINE (4)
#define DATE_WIDTH (200)
#define DATE_PADDING (4)
void draw_body(int rows, int display_date_offset) {
    int date_width = (GetScreenWidth() - 2 * BODY_PADDING_OUT - (DAYS_IN_WEEK - 1) * BODY_PADDING_IN) / DAYS_IN_WEEK;
    int text_size = find_ideal_text_size(TEST_DATE_STR, date_width - 2 * DATE_OUTLINE - 2 * DATE_PADDING);
    int week_bar_height = text_size + BODY_PADDING_IN;
    int date_height = (GetScreenHeight() - TOP_BAR_HEIGHT - week_bar_height - 2 * BODY_PADDING_OUT - (rows - 1) * BODY_PADDING_IN) / rows;
    for (int w = 0; w < DAYS_IN_WEEK; ++w) {
        DrawText(WEEKDAY_NAMES[w],
            BODY_PADDING_OUT + (date_width + BODY_PADDING_IN) * w + DATE_OUTLINE,
            TOP_BAR_HEIGHT + BODY_PADDING_OUT,
            text_size, BORDER_COLOR1);
    }
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < DAYS_IN_WEEK; ++x) {
            DrawRectangle(
                BODY_PADDING_OUT + (date_width + BODY_PADDING_IN) * x,
                TOP_BAR_HEIGHT + BODY_PADDING_OUT + week_bar_height + (date_height + BODY_PADDING_IN) * y, 
                date_width, date_height, BORDER_COLOR1);
            DrawRectangle(
                BODY_PADDING_OUT + (date_width + BODY_PADDING_IN) * x + DATE_OUTLINE,
                TOP_BAR_HEIGHT + BODY_PADDING_OUT + week_bar_height + (date_height + BODY_PADDING_IN) * y + DATE_OUTLINE,
                date_width - 2 * DATE_OUTLINE, date_height - 2 * DATE_OUTLINE, BG_COLOR2);
            int day_num = y * DAYS_IN_WEEK + x - display_date_offset;
            if (day_num < 0) {
                day_num = 30 + day_num;
            }
            day_num = day_num  % 30 + 1;
            char intstr[3]; // Always remember to leave a slot for the '\n' character
            snprintf(intstr, 3, "%d", day_num); // Like printf, but returns the string instread of printing it (and also has a max size for additional safety)
            DrawText(&intstr[0],
                BODY_PADDING_OUT + (date_width + BODY_PADDING_IN) * x + DATE_OUTLINE + DATE_PADDING,
                TOP_BAR_HEIGHT + BODY_PADDING_OUT + week_bar_height + DATE_OUTLINE + DATE_PADDING + (date_height + BODY_PADDING_IN) * y,
                text_size, BORDER_COLOR1);
        }
    }
}

#define WIN_MIN_WIDTH (800)
#define WIN_MIN_HEIGHT (400)
int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIN_MIN_WIDTH, WIN_MIN_HEIGHT, "Hacktoberfest");
    SetWindowMinSize(WIN_MIN_WIDTH, WIN_MIN_HEIGHT);
    
    DAY today = get_today_utc();
    CLOCK_TIME now = get_current_clock_utc();
    printf("Today: %d of %d %d. Time: %d:%d:%d\n", today.date.day, today.date.month, today.date.year, now.hour, now.minute, now.second);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BG_COLOR1);
            draw_top_bar(app.view_type == WEEK_VIEW ? 3 : 1);
            draw_body(app.view_type == WEEK_VIEW ? 1 : 5, 1);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
