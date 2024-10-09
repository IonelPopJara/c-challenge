/**
 * I'm so sorry for everything I've done.
 *
 * With love and regret,
 * Tuhkis
 */

// Please don't mind the spaghetti in the code, I'm italian - M37

// May you find strength to endure what I have created /wanders

#include "raylib.h"
#include "stdio.h" // for printf (and now also snprintf i guess :P)
#include "string.h" // for strcopy and strcat

#include "app.h"
#include "day.h"
#include "dyn_array.h"
#include "style.h"
#include "ui.h"
#include "util.h"
#include "math.h"

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

    // otherwise, month view is bugged if the first day is a sunday, and apparently the stuff above makes day = 2 when its a sunday, idfk, it just works /wanders
    if (app.view_first.date.day == 2) {
        app.view_first.date.day -= DAYS_IN_WEEK;
        validate_day(&app.view_first);
    }
}

void format_time(CLOCK_TIME time, char *buffer) {
    sprintf(buffer, "%02d:%02d", time.hour, time.minute);
}

void draw_centered_text(const char *text, float x, float y, float width, float height, int text_size, Color color) {
    int text_width = MeasureText(text, text_size);
    int text_height = text_size;
    float text_x = x + (width - text_width) / 2;
    float text_y = y + (height - text_height) / 2;
    DrawText(text, text_x, text_y, text_size, color);
}

#define TOP_BAR_HEIGHT (66)
#define TOP_BAR_PADDING (8)

#define HOME_WIDTH (48)
#define HOME_ICON_PADDING_OUT (4)
#define HOME_ICON_PADDING_IN (4)
#define HOME_X_POS (TOP_BAR_PADDING)
#define HOME_Y_POS (TOP_BAR_PADDING)
void home_button(Sound *click_sound) {
    DrawRectangle(HOME_X_POS, HOME_Y_POS, HOME_WIDTH, TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING, ACCENT_COLOR2);
    if (BUTTON_RELEASED == test_button(HOME_X_POS, HOME_Y_POS, HOME_WIDTH, TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING, MOUSE_BUTTON_LEFT)) {
        app.view_first = get_today_local();
        app.view_first.date.day -= (int)(app.view_first.weekday) - 1;
        validate_day(&app.view_first);
        if (app.view_type == MONTH_VIEW) {
            snap_focus_to_1st_of_month();
        }
        validate_day(&app.view_first);
        PlaySound(*click_sound);
    }

    // the icon (all code below in the function) is CoPilot magic :d /wanders
    // sue me, i used AI
    
    // New icon shape: a house
    int house_base_width = HOME_WIDTH - 2 * HOME_ICON_PADDING_OUT;
    int house_base_height = (TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING) / 2;
    int roof_height = house_base_height / 2;

    // Calculate center positions
    int center_x = HOME_X_POS + HOME_WIDTH / 2;
    int center_y = HOME_Y_POS + (TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING) / 2 + 5;

    // Draw house base
    DrawRectangle(
        center_x - house_base_width / 2,
        center_y - house_base_height / 2,
        house_base_width,
        house_base_height,
        BG_COLOR1
    );

    // Draw house roof
    DrawTriangle(
        (Vector2){center_x - house_base_width / 2, center_y - house_base_height / 2},
        (Vector2){center_x + house_base_width / 2, center_y - house_base_height / 2},
        (Vector2){center_x, center_y - house_base_height / 2 - roof_height},
        BG_COLOR1
    );
}

#define SELECT_VIEW_WIDTH (64)
#define SELECT_VIEW_ICON_COLUMNS (3)
#define SELECT_VIEW_ICON_PADDING_OUT (4)
#define SELECT_VIEW_ICON_PADDING_IN (4)
#define SELECT_VIEW_X_POS (HOME_X_POS + HOME_WIDTH + HOME_ICON_PADDING_OUT) 
#define SELECT_VIEW_Y_POS (TOP_BAR_PADDING)
void view_button(Sound *click_sound) {
    int rows = app.view_type == WEEK_VIEW ? 3 : 1;
    DrawRectangle(SELECT_VIEW_X_POS, TOP_BAR_PADDING, SELECT_VIEW_WIDTH, TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING, ACCENT_COLOR2);
    if (BUTTON_RELEASED == test_button(SELECT_VIEW_X_POS, TOP_BAR_PADDING, SELECT_VIEW_WIDTH, TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING,MOUSE_BUTTON_LEFT)) {
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
                SELECT_VIEW_X_POS + SELECT_VIEW_ICON_PADDING_OUT + (bar_width + SELECT_VIEW_ICON_PADDING_IN) * x,
                TOP_BAR_PADDING + SELECT_VIEW_ICON_PADDING_OUT + (bar_height + SELECT_VIEW_ICON_PADDING_IN) * y, 
                bar_width, bar_height, BG_COLOR1);
        }
    }
}

#define ARROW_BOX_WIDTH (32)
#define ARROW_PADDING (4)
#define ARROW_ICON_PADDING_OUT (4)
#define ARROW_X_POS (SELECT_VIEW_X_POS + SELECT_VIEW_WIDTH + SELECT_VIEW_ICON_PADDING_OUT)
#define ARROW_Y_POS (TOP_BAR_PADDING)
void arrows_buttons(Sound *click_sound) {
    DrawRectangle(ARROW_X_POS, TOP_BAR_PADDING, ARROW_BOX_WIDTH, TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING, ACCENT_COLOR2);
    
    Vector2 t1_v1;
    t1_v1.x = ARROW_X_POS + ARROW_BOX_WIDTH - 2 * ARROW_PADDING;
    t1_v1.y = TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING + ARROW_PADDING;
    Vector2 t1_v2;
    t1_v2.x = ARROW_X_POS + ARROW_BOX_WIDTH - 2 * ARROW_PADDING;
    t1_v2.y = TOP_BAR_PADDING + ARROW_PADDING;
    Vector2 t1_v3;
    t1_v3.x = ARROW_X_POS + ARROW_PADDING;
    t1_v3.y = TOP_BAR_HEIGHT / 2;
    
    DrawTriangle(t1_v1, t1_v2, t1_v3, BG_COLOR1);
    if (BUTTON_RELEASED == test_button(ARROW_X_POS, TOP_BAR_PADDING, ARROW_BOX_WIDTH, TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING,MOUSE_BUTTON_LEFT)) {
        app.view_first.date.day -= DAYS_IN_WEEK * (app.view_type == WEEK_VIEW ? 1 : WEEKS_IN_MONTH);
        validate_day(&app.view_first);
        if (app.view_type == MONTH_VIEW) {
            snap_focus_to_1st_of_month();
        }
        PlaySound(*click_sound);
    }
    
    DrawRectangle(ARROW_X_POS + ARROW_BOX_WIDTH + SELECT_VIEW_ICON_PADDING_OUT, TOP_BAR_PADDING, ARROW_BOX_WIDTH, TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING, ACCENT_COLOR2);
    
    Vector2 t2_v1 = t1_v1;
    t2_v1.x = ARROW_X_POS + ARROW_BOX_WIDTH + SELECT_VIEW_ICON_PADDING_OUT + 2 * ARROW_PADDING;
    t2_v1.y = TOP_BAR_PADDING + ARROW_PADDING;
    Vector2 t2_v2 = t1_v2;
    t2_v2.x = ARROW_X_POS + ARROW_BOX_WIDTH + SELECT_VIEW_ICON_PADDING_OUT + 2 * ARROW_PADDING;
    t2_v2.y = TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING + ARROW_PADDING;
    Vector2 t2_v3 = t1_v3;
    t2_v3.x = ARROW_X_POS + 2 * ARROW_BOX_WIDTH + SELECT_VIEW_ICON_PADDING_OUT - ARROW_PADDING;
    t2_v3.y = TOP_BAR_HEIGHT / 2;
    
    DrawTriangle(t2_v1, t2_v2, t2_v3, BG_COLOR1);
    if (BUTTON_RELEASED == test_button(ARROW_X_POS + ARROW_BOX_WIDTH + 2 * SELECT_VIEW_ICON_PADDING_OUT, TOP_BAR_PADDING, ARROW_BOX_WIDTH, TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING,MOUSE_BUTTON_LEFT)) {
        app.view_first.date.day += DAYS_IN_WEEK * (app.view_type == WEEK_VIEW ? 1 : WEEKS_IN_MONTH);
        validate_day(&app.view_first);
        if (app.view_type == MONTH_VIEW) {
            snap_focus_to_1st_of_month();
        }
        PlaySound(*click_sound);
    }
}

#define SETTINGS_WIDTH (48)
#define SETTINGS_ICON_PADDING_OUT (4)
#define SETTINGS_ICON_PADDING_IN (4)
#define SETTINGS_X_POS (ARROW_X_POS + 2 * ARROW_BOX_WIDTH + 2 * ARROW_ICON_PADDING_OUT)
#define SETTINGS_Y_POS (TOP_BAR_PADDING)
static int settings_open = 0;
void settings_button(Sound *click_sound) {
    DrawRectangle(SETTINGS_X_POS, SETTINGS_Y_POS, SETTINGS_WIDTH, TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING, ACCENT_COLOR2);
    if (BUTTON_RELEASED == test_button(SETTINGS_X_POS, SETTINGS_Y_POS, SETTINGS_WIDTH, TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING,MOUSE_BUTTON_LEFT)) {
        PlaySound(*click_sound);
        settings_open = !settings_open;
    }

    // the icon (all code below in the function) is CoPilot magic :d /wanders
    // sue me, i used AI 2x

    // New icon shape: a detailed gear
    int gear_radius = (SETTINGS_WIDTH - 2 * SETTINGS_ICON_PADDING_OUT) / 2;
    int gear_teeth = 12; // More teeth for a detailed look
    int gear_inner_radius = gear_radius / 2;
    int gear_center_x = SETTINGS_X_POS + SETTINGS_WIDTH / 2;
    int gear_center_y = SETTINGS_Y_POS + (TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING) / 2;
    int tooth_depth = gear_radius / 4; // Depth of each tooth
    
    for (int i = 0; i < gear_teeth; i++) {
        float angle = i * 2 * PI / gear_teeth;
        float next_angle = (i + 1) * 2 * PI / gear_teeth;
        float mid_angle = (angle + next_angle) / 2;
    
        Vector2 p1 = {gear_center_x + gear_radius * cos(angle), gear_center_y + gear_radius * sin(angle)};
        Vector2 p2 = {gear_center_x + gear_radius * cos(next_angle), gear_center_y + gear_radius * sin(next_angle)};
        Vector2 p3 = {gear_center_x + (gear_radius - tooth_depth) * cos(next_angle), gear_center_y + (gear_radius - tooth_depth) * sin(next_angle)};
        Vector2 p4 = {gear_center_x + (gear_radius - tooth_depth) * cos(mid_angle), gear_center_y + (gear_radius - tooth_depth) * sin(mid_angle)};
        Vector2 p5 = {gear_center_x + (gear_radius - tooth_depth) * cos(angle), gear_center_y + (gear_radius - tooth_depth) * sin(angle)};
        Vector2 p6 = {gear_center_x + gear_inner_radius * cos(angle), gear_center_y + gear_inner_radius * sin(angle)};
        Vector2 p7 = {gear_center_x + gear_inner_radius * cos(next_angle), gear_center_y + gear_inner_radius * sin(next_angle)};
    
        DrawTriangle(p1, p2, p3, BG_COLOR1);
        DrawTriangle(p1, p3, p4, BG_COLOR1);
        DrawTriangle(p1, p4, p5, BG_COLOR1);
        DrawTriangle(p5, p6, p7, BG_COLOR1);
    }
    
    DrawCircle(gear_center_x, gear_center_y, gear_inner_radius, BG_COLOR1);
    
    // Add inner circles for more detail
    int inner_circle_radius = gear_inner_radius / 3;
    DrawCircle(gear_center_x, gear_center_y, inner_circle_radius, BG_COLOR2);
    inner_circle_radius /= 2;
    DrawCircle(gear_center_x, gear_center_y, inner_circle_radius, BG_COLOR2);
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
    home_button(click_sound);
    view_button(click_sound);
    arrows_buttons(click_sound);
    settings_button(click_sound);
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
    static int is_today = 0;
    static char title[SCHEDULE_TITLE_MAX_LEN + 1] = "\0";
    static int title_letter_count = 0;
    static char description[SCHEDULE_TEXT_MAX_LEN + 1] = "\0";
    static int schedule_letter_count = 0;
    static int wrap_title_index = 0;
    static float key_hold_time = 0.0f;
    const float hold_threshold = 0.5f;
    const float delete_interval = 0.05f; 
    
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
            }
            
            char intstr[3]; // Always remember to leave a slot for the '\n' character
            snprintf(intstr, 3, "%d", current.date.day); // Like printf, but returns the string instead of printing it (and also has a max size for additional safety)
            
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
                    last_pressed_day = current;
                    if (current.date.year == today.date.year && current.date.month == today.date.month && current.date.day == today.date.day) {
                        is_today = 1;
                    } else {
                        is_today=0;
                    }
                } else if (is_menu_visible == 1 && !mouse_on_title) {
                  is_menu_visible=0;
                }
            }
            if ((BUTTON_RELEASED == test_button(0,0,GetScreenWidth(),TOP_BAR_HEIGHT,MOUSE_BUTTON_LEFT))) {
                is_menu_visible = 0;
            }
            int prev_day = current.date.day;
            ++current.date.day;
            validate_day(&current);
            if (app.view_type == MONTH_VIEW && is_day_of_current_month == 1 && prev_day > current.date.day) {
                is_day_of_current_month = 2;
            }
        }
    }

    if (settings_open) {
        int width = GetScreenWidth();
        int height = GetScreenHeight();

        // Draw the settings window
        int settings_window_width = 300;
        int settings_window_height = 200;
        int settings_window_x = width / 2 - settings_window_width / 2;
        int settings_window_y = height / 2 - settings_window_height / 2;
        DrawRectangle(settings_window_x, settings_window_y, settings_window_width, settings_window_height, ACCENT_COLOR1);

        // Draw caption
        int caption_height = 30;
        int caption_x = settings_window_x + settings_window_width / 2;
        int caption_y = settings_window_y + 10;
        DrawText("Settings", caption_x - MeasureText("Settings", 20) / 2, caption_y, 20, BG_COLOR1);

        // Customizable distance between text and checkbox
        int distance_between_text_and_checkbox = 10;
    
        // Draw text and checkbox on the same horizontal plane
        int checkbox_size = 20;
        int text_width = MeasureText("Clock format", 20);
        int total_width = text_width + distance_between_text_and_checkbox + checkbox_size;

        int text_x = settings_window_x + (settings_window_width - total_width) / 2;
        int text_y = caption_y + caption_height + 30; // Adjust vertical position as needed
        DrawText("Clock format", text_x, text_y, 20, BG_COLOR1);

        int checkbox_x = text_x + text_width + distance_between_text_and_checkbox;
        int checkbox_y = text_y + (20 - checkbox_size) / 2; // Center checkbox vertically with text
        DrawRectangle(checkbox_x, checkbox_y, checkbox_size, checkbox_size, BG_COLOR1);

        if (app.use_24h_format) {
            DrawLine(checkbox_x, checkbox_y, checkbox_x + checkbox_size, checkbox_y + checkbox_size, ACCENT_COLOR2);
            DrawLine(checkbox_x + checkbox_size, checkbox_y, checkbox_x, checkbox_y + checkbox_size, ACCENT_COLOR2);
        }

        if (BUTTON_RELEASED == test_button(checkbox_x, checkbox_y, checkbox_size, checkbox_size, MOUSE_BUTTON_LEFT)) {
            app.use_24h_format = !app.use_24h_format;
        }
    }

    else if (is_menu_visible) {
        // Day Schedule Box
        DrawRectangle(GetScreenWidth() / 4,
                      (GetScreenHeight() + 2 * TOP_BAR_HEIGHT) / 4,
                      GetScreenWidth() / 2, GetScreenHeight() / 2, ACCENT_COLOR1);
        char str[40];
        snprintf(str, 40, "Task for %s",
                 is_today ? "Today"
                          : TextFormat("%d/%d/%d", last_pressed_day.date.day,
                                       last_pressed_day.date.month,
                                       last_pressed_day.date.year));
        /*
            Center a object(horizontal or vertical) in a given rectangle box
            x = X + (W-w)/2
            x -> centre position , X-> posiiton of rectangle , W = width of rectangle , w = width of  object
        */
        int text_pos = (GetScreenWidth() / 4) +
                       (GetScreenWidth() / 2 - MeasureText(str, text_size)) / 2;
                    

        DrawText(str, text_pos, (GetScreenHeight() + 2 * TOP_BAR_HEIGHT) / 4 + 10,
                 text_size, BG_COLOR2);

        // Day Schedule Title Box
        Rectangle title_box = {(float)GetScreenWidth() / 4 + 20,
                               (float)(GetScreenHeight() + 2 * TOP_BAR_HEIGHT) /4 + (float)GetScreenHeight() / 12,
                               (float)GetScreenWidth() / 2 - 40,
                               (float)GetScreenHeight() / 12};
        // Rectangle dit_details_box = {
        //             .x = GetScreenWidth() / 4 + 20,
        //             .y = GetScreenHeight()+ 2*TOP_BAR_HEIGHT/4+GetScreenHeight()/12
        //           };


        DrawRectangleRec(title_box, BORDER_COLOR1);
       

        Vector2 title_pos = {
            .x = (float)(GetScreenWidth()) / 4 + 20 +
                 (float)((GetScreenWidth() / 2) - 40 -
                         (MeasureText("Enter Title", text_size))) /
                     2,
            .y = (float)(GetScreenHeight() + 2 * TOP_BAR_HEIGHT) / 4 +
                 (float)GetScreenHeight() / 12 +
                 ((float)GetScreenHeight() / 12 - text_size) / 2};

        // Only draw text if there is not any input from the user yet.
        if (title_letter_count == 0) {
            DrawText("Enter Title", title_pos.x, title_pos.y, text_size,
                 ColorAlpha(BG_COLOR2, 0.6));
        }

        if (CheckCollisionPointRec(GetMousePosition(), title_box))
            mouse_on_title = 1;
        else
            mouse_on_title = 0;

        if (mouse_on_title) {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();
            while (key > 0) {
            // NOTE: Only allow keys in range [32..125]
                if ((key >= 32) && (key <= 125) && (title_letter_count < SCHEDULE_TITLE_MAX_LEN)) {
                    title[title_letter_count] = (char)key;
                    title[title_letter_count + 1] = '\0';
                    title_letter_count++;
                    if (MeasureText(title, text_size) + text_size + 5 > ((int)title_box.width)) {
                        wrap_title_index++;
                    } else {
                        wrap_title_index = 0;
                    }
                }
            key = GetCharPressed();
            }

            // If backspace is pressed, delete the last character
            if (IsKeyPressed(KEY_BACKSPACE)) {
                wrap_title_index--;
                if (wrap_title_index < 0 ) {
                    wrap_title_index = 0;
                }
                title_letter_count--;
                if (title_letter_count < 0) {
                    title_letter_count = 0;
                }
                title[title_letter_count] = '\0';

                key_hold_time = 0.0f; 
            }

            // If backspace is held, delete the last character continuously
            if (IsKeyDown(KEY_BACKSPACE)) {
                key_hold_time += GetFrameTime();
                if (key_hold_time >= hold_threshold) {

                    key_hold_time = key_hold_time - delete_interval;

                    wrap_title_index--;
                    if (wrap_title_index < 0 ) {
                        wrap_title_index = 0;
                    }
                    title_letter_count--;
                    if (title_letter_count < 0) {
                        title_letter_count = 0;
                    }
                    title[title_letter_count] = '\0';
                    
                }
            } 

            else {
                key_hold_time = 0.0f;
            }

            if (IsKeyPressed(KEY_ENTER) && title_letter_count > 0) {
                SCHEDULE_ITEM item = {0};
                strcpy(item._title, title);
                strcpy(item._description, description);

                item.begin_time = get_current_clock_local(); // temporary
                // + randomize a few minutes. otherwise, all items will be on the same time, and that doesnt work
                item.begin_time.minute += GetRandomValue(0, 59); // also, temporary :D
                item.duration.hours = 1; // temporary
                item.duration.minutes = 0; // temporary

                add_schedule_item(&last_pressed_day, item);

                title[0] = '\0';
                description[0] = '\0';
                title_letter_count = 0;
                schedule_letter_count = 0;
                wrap_title_index = 0;
            }

        } else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        /**
         * Basically, all in this 'if-statement' checks if there are any items in the schedule, and draws them, if you have the menu open.
         * Yes, is a mess.
         * 
         * CoPilot magic used for the coordinates.
         * sue me, i used AI 3x
         * 
         * /wanders
         */
        if (is_day_in_list(&last_pressed_day) && !is_day_empty(&last_pressed_day)) {
            SCHEDULE_ITEM *first_item, *second_item, *third_item;
            int first_index, second_index, third_index = -1;
            first_index = first_schedule_item(&last_pressed_day);
            first_item = get_schedule_item(&last_pressed_day, first_index);
            
            second_index = has_next_schedule_item(&last_pressed_day, first_item->begin_time);
            if (second_index != -1) {
                second_item = get_schedule_item(&last_pressed_day, second_index);
                third_index = has_next_schedule_item(&last_pressed_day, second_item->begin_time);
                if (third_index != -1) {
                    third_item = get_schedule_item(&last_pressed_day, third_index);
                }
            }
            // Calculate the y position for the items based on the title box
            float item_start_y = title_box.y + title_box.height + 10; // 10 pixels padding

            // Draw the first item
            DrawRectangleRec((Rectangle){.x = title_box.x,
                                         .y = item_start_y,
                                         .width = title_box.width,
                                         .height = title_box.height},
                            BORDER_COLOR1);

            draw_centered_text(first_item->_title, title_box.x, item_start_y, title_box.width, title_box.height / 2, text_size, BG_COLOR2);

            // Draw the starting time and duration
            char start_time_str[6], duration_str[10];
            format_time(first_item->begin_time, start_time_str);
            sprintf(duration_str, "%02d:%02d", first_item->duration.hours, first_item->duration.minutes);
            char time_duration_str[40];
            sprintf(time_duration_str, "Start: %s, Duration: %s", start_time_str, duration_str);

            draw_centered_text(time_duration_str, title_box.x, item_start_y + title_box.height / 2, title_box.width, title_box.height / 2, text_size, BG_COLOR1); // Different color for time and duration

            // Is there a second item?
            if (second_index != -1) {
                // Draw the second item
                DrawRectangleRec((Rectangle){.x = title_box.x,
                                             .y = item_start_y + title_box.height + 10, // 10 pixels padding
                                             .width = title_box.width,
                                             .height = title_box.height},
                                BORDER_COLOR1);

                draw_centered_text(second_item->_title, title_box.x, item_start_y + title_box.height + 10, title_box.width, title_box.height / 2, text_size, BG_COLOR2);

                // Draw the starting time and duration for the second item
                format_time(second_item->begin_time, start_time_str);
                sprintf(duration_str, "%02d:%02d", second_item->duration.hours, second_item->duration.minutes);
                sprintf(time_duration_str, "Start: %s, Duration: %s", start_time_str, duration_str);

                draw_centered_text(time_duration_str, title_box.x, item_start_y + title_box.height + 10 + title_box.height / 2, title_box.width, title_box.height / 2, text_size, BG_COLOR1); // Different color for time and duration
            }

            // Is there a third item?
            if (third_index != -1) {
                // Draw the third item
                DrawRectangleRec((Rectangle){.x = title_box.x,
                                             .y = item_start_y + 2 * (title_box.height + 10), // 10 pixels padding
                                             .width = title_box.width,
                                             .height = title_box.height},
                                BORDER_COLOR1);

                draw_centered_text(third_item->_title, title_box.x, item_start_y + 2 * (title_box.height + 10), title_box.width, title_box.height / 2, text_size, BG_COLOR2);

                // Draw the starting time and duration for the third item
                format_time(third_item->begin_time, start_time_str);
                sprintf(duration_str, "%02d:%02d", third_item->duration.hours, third_item->duration.minutes);
                sprintf(time_duration_str, "Start: %s, Duration: %s", start_time_str, duration_str);

                draw_centered_text(time_duration_str, title_box.x, item_start_y + 2 * (title_box.height + 10) + title_box.height / 2, title_box.width, title_box.height / 2, text_size, BG_COLOR1); // Different color for time and duration
            }
        }

        DrawText(&title[wrap_title_index], title_box.x + 5, title_pos.y,
                 text_size, ACCENT_COLOR2);

        if (mouse_on_title) {
            CLOCK_TIME now = get_current_clock_local();
                if (title_letter_count < SCHEDULE_TITLE_MAX_LEN) {
                    if (now.second % 2 == 0) {
                        if (wrap_title_index > 0) {
                            DrawText("_", title_box.width + title_box.x - text_size,
                                     title_box.y + title_box.height - text_size, text_size,
                                     ACCENT_COLOR2);
                        } else {
                            DrawText("_", title_box.x + 10 + MeasureText(title, text_size),
                                     title_box.y + title_box.height - text_size, text_size,
                                     ACCENT_COLOR2);
                        }
                    }
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
    SetTargetFPS(60);
    
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
