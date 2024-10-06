/**
 * I'm so sorry for everything I've done.
 *
 * With love and regret,
 * Tuhkis
 */

// Please don't mind the spaghetti in the code, I'm italian - M37

#include "raylib.h"
#include "stdio.h" // for printf (and now also snprintf i guess :P)
#include "string.h" // for strcopy and strcat

#include "app.h"
#include "day.h"
#include "dyn_array.h"
#include "style.h"
#include "ui.h"
#include "util.h"

// THIS IS PUBLICLY ACCESSED. DO NOT ACCESS FROM THREADS IF YOU DON't KNOW WHAT YOU'RE DOING
APP_STATE app = {0};

void snap_focus_to_1st_of_month() {
    if (app.view_first.date.day > 3 * DAYS_IN_WEEK) {
                app.view_first.date.month += 1;
            }
            app.view_first.date.day = 1;
            validate_day(&app.view_first);
            app.view_first.date.day -= (int)(app.view_first.weekday) - 1;
            validate_day(&app.view_first);
}

#define TOP_BAR_HEIGHT (66)
#define TOP_BAR_PADDING (8)

#define SELECT_VIEW_WIDTH (64)
#define SELECT_VIEW_ICON_COLUMNS (3)
#define SELECT_VIEW_ICON_PADDING_OUT (4)
#define SELECT_VIEW_ICON_PADDING_IN (4)
void view_button(Sound *click_sound) {
    int rows = app.view_type == WEEK_VIEW ? 3 : 1;
    DrawRectangle(TOP_BAR_PADDING, TOP_BAR_PADDING, SELECT_VIEW_WIDTH, TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING, ACCENT_COLOR2);
    if (BUTTON_RELEASED == test_button(TOP_BAR_PADDING, TOP_BAR_PADDING, SELECT_VIEW_WIDTH, TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING,MOUSE_BUTTON_LEFT)) {
        if (app.view_type == WEEK_VIEW) {
            app.view_type = MONTH_VIEW;
            snap_focus_to_1st_of_month();
        }
        else {
            app.view_type = WEEK_VIEW;
        }
        PlaySound(*click_sound);
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

#define ARROW_BOX_WIDTH (32)
#define ARROW_PADDING (4)

void arrows_buttons(Sound *click_sound) {
    DrawRectangle(TOP_BAR_PADDING + SELECT_VIEW_WIDTH + SELECT_VIEW_ICON_PADDING_OUT, TOP_BAR_PADDING, ARROW_BOX_WIDTH, TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING, ACCENT_COLOR2);
    
    Vector2 t1_v1;
    t1_v1.x = TOP_BAR_PADDING + SELECT_VIEW_WIDTH + SELECT_VIEW_ICON_PADDING_OUT + ARROW_BOX_WIDTH - 2 * ARROW_PADDING;
    t1_v1.y = TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING + ARROW_PADDING;
    Vector2 t1_v2;
    t1_v2.x = TOP_BAR_PADDING + SELECT_VIEW_WIDTH + SELECT_VIEW_ICON_PADDING_OUT + ARROW_BOX_WIDTH - 2 * ARROW_PADDING;
    t1_v2.y = TOP_BAR_PADDING + ARROW_PADDING;
    Vector2 t1_v3;
    t1_v3.x = TOP_BAR_PADDING + SELECT_VIEW_WIDTH + SELECT_VIEW_ICON_PADDING_OUT + ARROW_PADDING;
    t1_v3.y = TOP_BAR_HEIGHT / 2;
    
    DrawTriangle(t1_v1, t1_v2, t1_v3, BG_COLOR1);
    if (BUTTON_RELEASED == test_button(TOP_BAR_PADDING + SELECT_VIEW_WIDTH + SELECT_VIEW_ICON_PADDING_OUT, TOP_BAR_PADDING, ARROW_BOX_WIDTH, TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING,MOUSE_BUTTON_LEFT)) {
        app.view_first.date.day -= DAYS_IN_WEEK * (app.view_type == WEEK_VIEW ? 1 : WEEKS_IN_MONTH);
        validate_day(&app.view_first);
        PlaySound(*click_sound);
    }
    
    DrawRectangle(TOP_BAR_PADDING + SELECT_VIEW_WIDTH + ARROW_BOX_WIDTH + 2 * SELECT_VIEW_ICON_PADDING_OUT, TOP_BAR_PADDING, ARROW_BOX_WIDTH, TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING, ACCENT_COLOR2);
    
    Vector2 t2_v1 = t1_v1;
    t2_v1.x = TOP_BAR_PADDING + SELECT_VIEW_WIDTH + ARROW_BOX_WIDTH + 2 * SELECT_VIEW_ICON_PADDING_OUT + 2 * ARROW_PADDING;
    t2_v1.y = TOP_BAR_PADDING + ARROW_PADDING;
    Vector2 t2_v2 = t1_v2;
    t2_v2.x = TOP_BAR_PADDING + SELECT_VIEW_WIDTH + ARROW_BOX_WIDTH + 2 * SELECT_VIEW_ICON_PADDING_OUT + 2 * ARROW_PADDING;
    t2_v2.y = TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING + ARROW_PADDING;
    Vector2 t2_v3 = t1_v3;
    t2_v3.x = TOP_BAR_PADDING + SELECT_VIEW_WIDTH + 2 * ARROW_BOX_WIDTH + 2 * SELECT_VIEW_ICON_PADDING_OUT - ARROW_PADDING;
    t2_v3.y = TOP_BAR_HEIGHT / 2;
    
    DrawTriangle(t2_v1, t2_v2, t2_v3, BG_COLOR1);
    if (BUTTON_RELEASED == test_button(TOP_BAR_PADDING + SELECT_VIEW_WIDTH + ARROW_BOX_WIDTH + 2 * SELECT_VIEW_ICON_PADDING_OUT, TOP_BAR_PADDING, ARROW_BOX_WIDTH, TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING,MOUSE_BUTTON_LEFT)) {
        app.view_first.date.day += DAYS_IN_WEEK * (app.view_type == WEEK_VIEW ? 1 : WEEKS_IN_MONTH);
        validate_day(&app.view_first);
        PlaySound(*click_sound);
    }
}

#define DATE_YEAR_LABEL_SIZE (48)
#define TIME_LABEL_SIZE (48)
#define TIME_LABEL_PERIOD_SIZE (24)
#define COLUMN_CHAR_PADDING (3)
void time_label(Sound *click_sound) {
    CLOCK_TIME now = get_current_clock_local();
    char *time_period_str = app.use_24h_format == 1 ? "" : now.hour > 12 ? " PM" : " AM";
    char hour_str[3]; // Always remember to leave a slot for the '\n' character
    char minute_str[3]; // Always remember to leave a slot for the '\n' character
    snprintf(hour_str, 3, "%02d", app.use_24h_format == 0 && now.hour > 12 ? now.hour - 12 :  now.hour);
    snprintf(minute_str, 3, "%02d", now.minute);
    int hour_str_length = MeasureText(&hour_str[0], TIME_LABEL_SIZE);
    int dot_str_length = MeasureText(":", TIME_LABEL_SIZE);
    int minute_str_length = MeasureText(&minute_str[0], TIME_LABEL_SIZE);
    int timer_period_str_length = MeasureText(time_period_str, TIME_LABEL_PERIOD_SIZE);
    
    if (BUTTON_RELEASED == test_button(
        GetScreenWidth() - 2 * TOP_BAR_PADDING - timer_period_str_length - minute_str_length - dot_str_length - 2 * COLUMN_CHAR_PADDING - hour_str_length, 
        TOP_BAR_HEIGHT - TOP_BAR_PADDING - TIME_LABEL_SIZE, 
        hour_str_length + dot_str_length + minute_str_length + timer_period_str_length + 2 * COLUMN_CHAR_PADDING, 
        TIME_LABEL_SIZE,MOUSE_BUTTON_LEFT)) {
        app.use_24h_format = ++app.use_24h_format % 2;
        PlaySound(*click_sound);
    }
    
    DrawText(time_period_str,
        GetScreenWidth() - 2 * TOP_BAR_PADDING - timer_period_str_length,
        TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING - TIME_LABEL_PERIOD_SIZE,
        TIME_LABEL_PERIOD_SIZE, ACCENT_COLOR2);
    DrawText(&minute_str[0],
        GetScreenWidth() - 2 * TOP_BAR_PADDING - timer_period_str_length - minute_str_length,
        TOP_BAR_HEIGHT - TOP_BAR_PADDING - TIME_LABEL_SIZE, 
        TIME_LABEL_SIZE, ACCENT_COLOR2);
    if (now.second % 2 == 0)
    {
        DrawText(":",
        GetScreenWidth() - 2 * TOP_BAR_PADDING - timer_period_str_length - minute_str_length - dot_str_length - COLUMN_CHAR_PADDING,
        TOP_BAR_HEIGHT - TOP_BAR_PADDING - TIME_LABEL_SIZE, 
        TIME_LABEL_SIZE, ACCENT_COLOR2);
    }
    DrawText(&hour_str[0],
        GetScreenWidth() - 2 * TOP_BAR_PADDING - timer_period_str_length - minute_str_length - dot_str_length - 2 * COLUMN_CHAR_PADDING - hour_str_length,
        TOP_BAR_HEIGHT - TOP_BAR_PADDING - TIME_LABEL_SIZE, 
        TIME_LABEL_SIZE, ACCENT_COLOR2);
}

// The W and the 8 are (usually) the widest characters among letters and numbers
#define TEST_DATE_STR ("WWW 88ww")
// This should have been declared inside of the "day" file, but i don't know how to make it visible here if i do that - M37
const char *WEEKDAY_NAMES[] = {
    "Monday", "Tuesday", 
    "Wednesday", "Thursday",
    "Friday", "Saturday", "Sunday"
};
// Same thing for this one as the prev. array - M37
const char *MONTH_NAMES[] = {
    "Jan.", "Feb.", "Mar.",
    "Apr.", "May", "June", "July",
    "Aug.", "Sept.", "Oct.", 
    "Nov.", "Dec."
};

void draw_top_bar(Sound *click_sound) {
    DrawRectangle(0, 0, GetScreenWidth(), TOP_BAR_HEIGHT, ACCENT_COLOR1);
    view_button(click_sound);
    arrows_buttons(click_sound);
    time_label(click_sound);
    
    DAY current = app.view_first;
    if (current.date.day > 3 * DAYS_IN_WEEK) {
        current.date.month += 1;
        validate_day(&current);
    }
    char date_year_str[14]; // Always remember to leave a slot for the '\n' character
    snprintf(date_year_str, 14, "%s %d", MONTH_NAMES[current.date.month - 1], current.date.year);
    
    DrawText(date_year_str,
        GetScreenWidth() / 2 - MeasureText(date_year_str, DATE_YEAR_LABEL_SIZE) / 2,
        TOP_BAR_HEIGHT - TOP_BAR_PADDING - DATE_YEAR_LABEL_SIZE, 
        DATE_YEAR_LABEL_SIZE, ACCENT_COLOR2);
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

#define BODY_PADDING_OUT (8)
#define BODY_PADDING_IN (2)
#define DATE_OUTLINE (4)
#define DATE_WIDTH (200)
#define DATE_PADDING (4)
#define MIN_DAY_MENU_WIDTH (80)
#define MIN_DAY_MENU_HEIGHT (40)
void draw_body(DAY today) {
    int rows = app.view_type == WEEK_VIEW ? 1 : 6;
    
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
    
    int is_day_of_current_month = (int)(app.view_type == WEEK_VIEW);
    DAY current = app.view_first;

    static int is_menu_visible = 0;
    static Vector2 last_mouse_pos = {0};
    static DAY last_pressed_day = {0};
    static int mouse_on_title=0;
    int is_today = 0;
    static char title[SCHEDULE_TITLE_MAX_LEN + 1] = "\0";
    static int title_letter_count = 0;
    static char description[SCHEDULE_TEXT_MAX_LEN + 1] = "\0";
    static int schedule_letter_count = 0;
    static int wrap_title_index = 0;
    
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < DAYS_IN_WEEK; ++x) {
            if (app.view_type == MONTH_VIEW && is_day_of_current_month == 0 && current.date.day == 1) {
                is_day_of_current_month = 1;
            }
            Color outline = BORDER_COLOR2;
            Color inner = BORDER_COLOR1;
            if (is_day_of_current_month == 1) {
                outline = BORDER_COLOR1;
                inner = BG_COLOR2;
            }
            // Override Color if drawing current day
            if (current.date.year == today.date.year && current.date.month == today.date.month && current.date.day == today.date.day) {
                outline = ACCENT_COLOR1;
                is_today = 1;
            } else {
                is_today=0;
            }
            
            char intstr[3]; // Always remember to leave a slot for the '\n' character
            snprintf(intstr, 3, "%d", current.date.day); // Like printf, but returns the string instread of printing it (and also has a max size for additional safety)
            
            DrawRectangle(
                BODY_PADDING_OUT + (date_width + BODY_PADDING_IN) * x,
                TOP_BAR_HEIGHT + BODY_PADDING_OUT + week_bar_height + (date_height + BODY_PADDING_IN) * y, 
                date_width, date_height, outline);
            DrawRectangle(
                BODY_PADDING_OUT + (date_width + BODY_PADDING_IN) * x + DATE_OUTLINE,
                TOP_BAR_HEIGHT + BODY_PADDING_OUT + week_bar_height + (date_height + BODY_PADDING_IN) * y + DATE_OUTLINE,
                date_width - 2 * DATE_OUTLINE, date_height - 2 * DATE_OUTLINE, inner);
            DrawText(&intstr[0],
                BODY_PADDING_OUT + (date_width + BODY_PADDING_IN) * x + DATE_OUTLINE + DATE_PADDING,
                TOP_BAR_HEIGHT + BODY_PADDING_OUT + week_bar_height + DATE_OUTLINE + DATE_PADDING + (date_height + BODY_PADDING_IN) * y,
                text_size, outline);


            
            if ((BUTTON_RELEASED == test_button(
                    BODY_PADDING_OUT + (date_width + BODY_PADDING_IN) * x + DATE_OUTLINE,
                    TOP_BAR_HEIGHT + BODY_PADDING_OUT + week_bar_height +
                        (date_height + BODY_PADDING_IN) * y + DATE_OUTLINE,
                    date_width - 2 * DATE_OUTLINE, date_height - 2 * DATE_OUTLINE, MOUSE_BUTTON_LEFT))){
                if (is_menu_visible == 0) {
                    //printf("On %d date\n", current.date.day);
                    is_menu_visible = 1;
                    last_pressed_day=current;
                } else if (is_menu_visible == 1 && !mouse_on_title) {
                  is_menu_visible=0;
                }
            }
            if ((BUTTON_RELEASED == test_button(0,0,GetScreenWidth(),TOP_BAR_HEIGHT,MOUSE_BUTTON_LEFT))) {
                is_menu_visible = 0;
            }
            if (is_menu_visible) {
                // Day Schedule Box
                DrawRectangle(GetScreenWidth() / 4,
                              (GetScreenHeight() + 2 * TOP_BAR_HEIGHT) / 4,
                              GetScreenWidth()/2,GetScreenHeight()/2, 
                              ACCENT_COLOR1);
                char str[40];
                snprintf(str, 40, "Task for %s", is_today?"Today":TextFormat("%d/%d/%d",last_pressed_day.date.day,last_pressed_day.date.month,last_pressed_day.date.year));
                // center the text in a given rectangle box
                int text_pos =
                    (GetScreenWidth() / 4) +
                    (GetScreenWidth() / 2 - MeasureText(str, text_size)) / 2;

                DrawText(str, text_pos,
                         (GetScreenHeight() + 2 * TOP_BAR_HEIGHT) / 4 + 10, text_size,
                         BG_COLOR2);

                // Day Schedule Title Box
                Rectangle title_box = {
                    (float)GetScreenWidth() / 4 + 20,
                    (float)(GetScreenHeight() + 2 * TOP_BAR_HEIGHT) / 4 + (float)GetScreenHeight()/12,
                    (float)GetScreenWidth() / 2 - 40, (float)GetScreenHeight() / 12};

                DrawRectangleRec(title_box, BORDER_COLOR1);

                Vector2 title_pos = {
                        .x = (float)(GetScreenWidth()) / 4 + 20 + (float)((GetScreenWidth() / 2) - 40 - (MeasureText("Title", text_size))) / 2,
                        .y = (float)(GetScreenHeight() + 2 * TOP_BAR_HEIGHT) / 4 +(float)GetScreenHeight() / 12 + ((float)GetScreenHeight() / 12 - text_size) / 2
                    };

                DrawText("Title", title_pos.x, title_pos.y,
                         text_size, ColorAlpha(BG_COLOR2, 0.6));

                if (CheckCollisionPointRec(GetMousePosition(), title_box)) mouse_on_title = 1;
                else mouse_on_title = 0;

                if (mouse_on_title) {
                    SetMouseCursor(MOUSE_CURSOR_IBEAM);
                    
                    int key = GetCharPressed();
                    while (key > 0){
                        // NOTE: Only allow keys in range [32..125]
                        if ((key >= 32) && (key <= 125) && (title_letter_count < SCHEDULE_TEXT_MAX_LEN))
                        {
                            title[title_letter_count] = (char)key;
                            title[title_letter_count + 1] = '\0';
                            title_letter_count++;
                        }
                        key = GetCharPressed();
                    }
                    // Todo : Fix Backspace behavior ( Removes multiples characters at once instead of one)
                    if (IsKeyPressed(KEY_BACKSPACE)){
                        title_letter_count--;
                        if (title_letter_count < 0) title_letter_count = 0;
                        title[title_letter_count] = '\0';
                    }
                } else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

                if (MeasureText(title, text_size) > (GetScreenWidth() / 2 - 40) ) {
                    int new_start = MeasureText(title, text_size) -
                                  (GetScreenWidth() / 2 - 40);
                    wrap_title_index++;
                    DrawText(&title[new_start], title_box.x+5, title_pos.y, text_size, ACCENT_COLOR2);
                }else{
                    DrawText(title, title_box.x + 5,title_pos.y,text_size,ACCENT_COLOR2);
                }
                if (mouse_on_title) {
                    CLOCK_TIME now = get_current_clock_local();
                    if (title_letter_count < SCHEDULE_TITLE_MAX_LEN) {
                        if (now.second % 2 == 0) {
                          DrawText("_",title_box.x + 8 + MeasureText(title, text_size),title_box.y + title_box.height - text_size,text_size, ACCENT_COLOR2);
                        }
                    }
                }

            }

            
            
            int prev_day = current.date.day;
            ++current.date.day;
            validate_day(&current);
            if (app.view_type == MONTH_VIEW && is_day_of_current_month == 1 && prev_day > current.date.day) {
                is_day_of_current_month = 2;
            }
        }
    }
}

void truncate_str_after_directory_separator(char *path) {
    for (int i = strlen(path); i >= 0; --i) {
        if (path[i] == '\\' || path[i] == '/') {
            path[i+1] = '\0';
            return;
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
    InitAudioDevice();
    
    /**
     * I'm sure there is a better way to optimize the memory usage for this, but currently, I am not aware of such way
     * I'm sorry for wasting your memory for such a simple task - M37
     */
    char local_path[strlen(argv[0]) + 1]; // Always remember to leave a slot for the '\n' character
    strcpy(local_path, argv[0]);
    truncate_str_after_directory_separator(local_path);
    
    char click_path[strlen(local_path) + 1]; // Always remember to leave a slot for the '\n' character
    strcpy(click_path, local_path);
    click_path[strlen(click_path) - 1] = '\0';
    truncate_str_after_directory_separator(click_path);
    #if defined(_WIN32)
        strcat(click_path, "assets\\click.wav");
    #elif unix
        strcat(click_path, "assets/click.wav");
    #endif
    Sound click_sound = LoadSound(click_path);
    
    app.use_24h_format = 1;
    
    DAY today = get_today_local();
    printf("Today: %d/%d/%d\n", today.date.day, today.date.month, today.date.year);
    
    app.view_first = today;
    // NOTE: Call snap_focus_to_1st_of_month() if setting the starting view_type as MONTH_VIEW
    app.view_first.date.day -= (int)(app.view_first.weekday) - 1;
    validate_day(&app.view_first);
    printf("First Monday: %d/%d/%d\n", app.view_first.date.day, app.view_first.date.month, app.view_first.date.year);
    
    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BG_COLOR1);
            draw_top_bar(&click_sound);
            draw_body(today);
        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
