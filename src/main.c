/**
 * I'm so sorry for everything I've done.
 *
 * With love and regret,
 * Tuhkis
 */

// Please don't mind the spaghetti in the code, I'm italian - M37

// May you find strength to endure what I have created /wanders

// I DID NOT USE ANY AI!!!!!!! 0 AI !! /eug-lena

/**
 * With all due respect, what the fuck.
 * - teo
 */

/**
 * And now I realize that this was the biggest mistake of my life
 * - Mults
 *
 * There were 1208 lines in main when I started my turn. My turn is
 * almost over, I've been only refactoring code, and somehow main
 * gained 63 lines. I blame the comments...
 *
 * I checked with cloc and this is what I got.
 *
 * Before:
 * -------------------------------------------------------------------------------
 * Language                     files          blank        comment code
 * -------------------------------------------------------------------------------
 * C                                1            161            185 862
 * -------------------------------------------------------------------------------
 *
 * After:
 * -------------------------------------------------------------------------------
 * Language                     files          blank        comment code
 * -------------------------------------------------------------------------------
 * C                                1            165            257 849
 * -------------------------------------------------------------------------------
 */

/** 
 * It took me around 4 minutes to understand that raylib is
 * absolutely not designed for UI
 * Thou shall witness the suff'ring of frame-bas'd graphics
 * - 13SHR
**/

/** KNOWN BUGS:
 * 1.
 * When cycling through the months, there is always a wrong pair. Meaning it
 *will say May twice in a row and not mention April. It shows the dates
 *correctly, and they change, but the month names are wrong. It's different
 *months for each year (kinda). Duplicated months: 2025 - May, 2026 - October,
 *2027 - July, 2028 - March, 2029 - March again ...
 *
 * 2.
 * Mouse inputs perpetuate through frames and interact with ui more than once :D
 * from lena: I added a close button to the settings menu which, if in year view
 *and clicked, will close the settings menu and open March and switch to month
 *view cause it's on top of it :D
 *
 * from lena: 3.
 * Sometimes the "home" button doesn't work. Example: It is Sunday, 13th Oct and
 *clicking the home button takes you to the next week. UPDATE: Should be fixed,
 *it doesn't work on Mondays though :)
 * UPDATE: It worked perfectly for me! - 13SHR
 *
 * 4. The start time of the tasks is randomly assigned and doesn't check if that
 *number of minutes is possible. Example: task starting at 11:64 CTRL + F: if
 *(IsKeyPressed(KEY_ENTER) && title_letter_count > 0)
 * UPDATE: This is now fixed thanks to Mults I think, and I added the ENTER key
 * from the numeric pad (so the ctrl + F won't work) - 13SHR
 *
 * 5. I (teo) think I managed to break the click sound when pressing buttons :(
 *          I'm not sure how to fix it, I'm sorry
 * Edit: it has something to do with the #ifdef _WIN32 in the main function,
 *adding another path the same way invalidates the gif Somewhere they are
 *interfering with each other Edit: (I think) I fixed this bug, the paths were
 *being assigned wrong sizes so the memory wasn't being allocated properly. -
 *Ionel
 *
 * 6. When the time format is not set to 24 hours, 12 p.m. and 12 p.m. are
 *reversed. If it's 00:00 it will display 12:00 p.m. I might've fixed it but I'm
 *not sure. You can check this function if you want `void time_label`.
 *
 * 7. For some reason whenever a task with no data other than the name is added,
 *no other task is displayed afterwards. For example, instead of adding "do
 *something;04:20pm;01:00" you add "do something", no other task is rendered :/
 *      I'm too tired to figure it out, i've been refactoring all day long.
 * UPDATE: The way the scheduled items were checked did not allow next items do
 * be properly displaid, because idk. So I added a special case when nothing
 * is given for time and duration, but the temporary solution of taking current
 * time is still there
 * - 13SHR
 * 
 **/

#include "app.h"
#include "day.h"
#include "dyn_array.h"
#include "raylib.h"
#include "stdio.h"   // for printf (and now also snprintf i guess :P)
#include "string.h"  // for strcopy and strcat
/** this is the old color scheme if ppl want to use it. it's spooky season so i
 * changed the color scheme */
//#include "classic_style.h"*/
#include <time.h>  // For time?? idk I'm not a clock maker

#include "math.h"
#include "spooky_style.h"
#include "ui.h"
#include "util.h"

// THIS IS PUBLICLY ACCESSED. DO NOT ACCESS FROM THREADS IF YOU DON't KNOW WHAT
// YOU'RE DOING
APP_STATE app = {0};
int show_welcome_message = 1;  // for the welcome message

void snap_focus_to_1st_of_month() {
  if (app.view_first.date.day > 3 * DAYS_IN_WEEK) {
    app.view_first.date.month += 1;
  }
  app.view_first.date.day = 1;
  validate_day(&app.view_first);
  app.view_first.date.day -= (int)(app.view_first.weekday) - 1;
  validate_day(&app.view_first);

  // otherwise, month view is bugged if the first day is a sunday, and
  // apparently the stuff above makes day = 2 when its a sunday, idfk, it just
  // works /wanders
  if (app.view_first.date.day == 2) {
    app.view_first.date.day -= DAYS_IN_WEEK;
    validate_day(&app.view_first);
  }
}

void snap_focus_to_1st_month_of_year() {
  app.view_first.date.month = 1;
  app.view_first.date.day = 1;
  validate_day(&app.view_first);
  app.view_first.date.day -= (int)(app.view_first.weekday) - 1;
  validate_day(&app.view_first);
}

void snap_focus_to_1st_day_of_current_week() {
  app.view_first = get_today_local();
  if (app.view_first.weekday == 0) {
    app.view_first.date.day -= 6;
  } else {
    app.view_first.date.day -= (int)(app.view_first.weekday) - 1;
  }
  validate_day(&app.view_first);
}

void format_time(CLOCK_TIME time, char* buffer) {
  sprintf(buffer, "%02d:%02d", time.hour, time.minute);
}

void draw_centered_text(const char* text, float x, float y, float width,
                        float height, int text_size, Color color) {
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
void home_button(Sound* click_sound) {
  DrawRectangle(HOME_X_POS, HOME_Y_POS, HOME_WIDTH,
                TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING, ACCENT_COLOR2);
  if (BUTTON_RELEASED == test_button(HOME_X_POS, HOME_Y_POS, HOME_WIDTH,
                                     TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING,
                                     MOUSE_BUTTON_LEFT)) {
    app.view_first = get_today_local();
    if (app.view_first.weekday ==
        0) {  // Assuming 0 represents Sunday, IDFK KNOW WHY IT WORKS EVEN
              // THOUGH MONDAY IS 0
      app.view_first.date.day -= 6;
    } else {
      app.view_first.date.day -= (int)(app.view_first.weekday) - 1;
    }
    validate_day(&app.view_first);
    if (app.view_type == MONTH_VIEW) {
      snap_focus_to_1st_of_month();
    }
    if (app.view_type == YEAR_VIEW) {
      app.view_type = WEEK_VIEW;
      snap_focus_to_1st_day_of_current_week();
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
  DrawRectangle(center_x - house_base_width / 2,
                center_y - house_base_height / 2, house_base_width,
                house_base_height, BG_COLOR1);

  // Draw house roof
  DrawTriangle(
      (Vector2){center_x - house_base_width / 2.,
                center_y - house_base_height / 2.},
      (Vector2){center_x + house_base_width / 2.,
                center_y - house_base_height / 2.},
      (Vector2){center_x, center_y - house_base_height / 2. - roof_height},
      BG_COLOR1);
}

#define SELECT_VIEW_WIDTH (64)
#define SELECT_VIEW_ICON_COLUMNS (4)
#define SELECT_VIEW_ICON_PADDING_OUT (4)
#define SELECT_VIEW_ICON_PADDING_IN (4)
#define SELECT_VIEW_X_POS (HOME_X_POS + HOME_WIDTH + HOME_ICON_PADDING_OUT)
#define SELECT_VIEW_Y_POS (TOP_BAR_PADDING)
void view_button(Sound* click_sound) {
  // int rows = app.view_type == WEEK_VIEW ? 3 : 1; // WTF DOES THIS MEAN
  // sorry to whoever wrote this, but I cannot for THE LIFE OF ME understand
  // ternary operators
  int rows;
  if (app.view_type == WEEK_VIEW) {
    // IF WEEK VIEW NEXT VIEW IS MONTH VIEW (2 ROWS)
    rows = 2;
  } else if (app.view_type == MONTH_VIEW) {
    // IF MONTH VIEW NEXT VIEW IS YEAR VIEW (3 ROWS)
    rows = 3;
  } else {
    rows = 1;
  }

  DrawRectangle(SELECT_VIEW_X_POS, TOP_BAR_PADDING, SELECT_VIEW_WIDTH,
                TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING, ACCENT_COLOR2);
  if (BUTTON_RELEASED ==
      test_button(SELECT_VIEW_X_POS, TOP_BAR_PADDING, SELECT_VIEW_WIDTH,
                  TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING, MOUSE_BUTTON_LEFT)) {
    if (app.view_type == WEEK_VIEW) {
      app.view_type = MONTH_VIEW;
      snap_focus_to_1st_of_month();
    } else if (app.view_type == MONTH_VIEW) {
      app.view_type = YEAR_VIEW;
      snap_focus_to_1st_month_of_year();
    } else {
      app.view_type = WEEK_VIEW;
      snap_focus_to_1st_day_of_current_week();
    }
    PlaySound(*click_sound);
  }

  int bar_width =
      (SELECT_VIEW_WIDTH - 2 * SELECT_VIEW_ICON_PADDING_OUT -
       (SELECT_VIEW_ICON_COLUMNS - 1) * SELECT_VIEW_ICON_PADDING_IN) /
      SELECT_VIEW_ICON_COLUMNS;
  int bar_height =
      (TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING - 2 * SELECT_VIEW_ICON_PADDING_OUT -
       (rows - 1) * SELECT_VIEW_ICON_PADDING_IN) /
      rows;
  for (int y = 0; y < rows; ++y) {
    for (int x = 0; x < SELECT_VIEW_ICON_COLUMNS; ++x) {
      DrawRectangle(SELECT_VIEW_X_POS + SELECT_VIEW_ICON_PADDING_OUT +
                        (bar_width + SELECT_VIEW_ICON_PADDING_IN) * x,
                    TOP_BAR_PADDING + SELECT_VIEW_ICON_PADDING_OUT +
                        (bar_height + SELECT_VIEW_ICON_PADDING_IN) * y,
                    bar_width, bar_height, BG_COLOR1);
    }
  }
}

#define ARROW_BOX_WIDTH (32)
#define ARROW_PADDING (4)
#define ARROW_ICON_PADDING_OUT (4)
#define ARROW_X_POS \
  (SELECT_VIEW_X_POS + SELECT_VIEW_WIDTH + SELECT_VIEW_ICON_PADDING_OUT)
#define ARROW_Y_POS (TOP_BAR_PADDING)
void arrows_buttons(Sound* click_sound) {
  DrawRectangle(ARROW_X_POS, TOP_BAR_PADDING, ARROW_BOX_WIDTH,
                TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING, ACCENT_COLOR2);

  Vector2 t1_v1;
  t1_v1.x = ARROW_X_POS + ARROW_BOX_WIDTH - 2 * ARROW_PADDING;
  t1_v1.y = TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING + ARROW_PADDING;
  Vector2 t1_v2;
  t1_v2.x = ARROW_X_POS + ARROW_BOX_WIDTH - 2 * ARROW_PADDING;
  t1_v2.y = TOP_BAR_PADDING + ARROW_PADDING;
  Vector2 t1_v3;
  t1_v3.x = ARROW_X_POS + ARROW_PADDING;
  t1_v3.y = TOP_BAR_HEIGHT / 2.;

  // Arrow pointing left
  DrawTriangle(t1_v1, t1_v2, t1_v3, BG_COLOR1);
  if (BUTTON_RELEASED ==
      test_button(ARROW_X_POS, TOP_BAR_PADDING, ARROW_BOX_WIDTH,
                  TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING, MOUSE_BUTTON_LEFT)) {
    if (app.view_type == WEEK_VIEW) {
      app.view_first.date.day -= DAYS_IN_WEEK * 1;
    } else if (app.view_type == MONTH_VIEW) {
      app.view_first.date.day -= DAYS_IN_WEEK * WEEKS_IN_MONTH;
      snap_focus_to_1st_of_month();
    } else {
      // switch to the previous year
      app.view_first.date.year -= 1;
      // snap to the first month of the year
      app.view_first.date.month = 1;
    }

    validate_day(&app.view_first);
    PlaySound(*click_sound);
  }

  DrawRectangle(ARROW_X_POS + ARROW_BOX_WIDTH + SELECT_VIEW_ICON_PADDING_OUT,
                TOP_BAR_PADDING, ARROW_BOX_WIDTH,
                TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING, ACCENT_COLOR2);

  Vector2 t2_v1 = t1_v1;
  t2_v1.x = ARROW_X_POS + ARROW_BOX_WIDTH + SELECT_VIEW_ICON_PADDING_OUT +
            2 * ARROW_PADDING;
  t2_v1.y = TOP_BAR_PADDING + ARROW_PADDING;
  Vector2 t2_v2 = t1_v2;
  t2_v2.x = ARROW_X_POS + ARROW_BOX_WIDTH + SELECT_VIEW_ICON_PADDING_OUT +
            2 * ARROW_PADDING;
  t2_v2.y = TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING + ARROW_PADDING;
  Vector2 t2_v3 = t1_v3;
  t2_v3.x = ARROW_X_POS + 2 * ARROW_BOX_WIDTH + SELECT_VIEW_ICON_PADDING_OUT -
            ARROW_PADDING;
  t2_v3.y = TOP_BAR_HEIGHT / 2.;

  // Arrow pointing right
  DrawTriangle(t2_v1, t2_v2, t2_v3, BG_COLOR1);
  if (BUTTON_RELEASED ==
      test_button(
          ARROW_X_POS + ARROW_BOX_WIDTH + 2 * SELECT_VIEW_ICON_PADDING_OUT,
          TOP_BAR_PADDING, ARROW_BOX_WIDTH,
          TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING, MOUSE_BUTTON_LEFT)) {
    if (app.view_type == WEEK_VIEW) {
      app.view_first.date.day += DAYS_IN_WEEK * 1;
    } else if (app.view_type == MONTH_VIEW) {
      app.view_first.date.day += DAYS_IN_WEEK * WEEKS_IN_MONTH;
    } else {
      // switch to the next year
      app.view_first.date.year += 1;
      // snap to the first month of the year
      app.view_first.date.month = 1;
    }

    validate_day(&app.view_first);
    if (app.view_type == MONTH_VIEW) {
      snap_focus_to_1st_of_month();
    }
    PlaySound(*click_sound);
  }
}

/**
 * This function draws the settings button in the top bar.
 * Opens settings menu when pressed (sets settings_open to 1/0).
 **/

#define SETTINGS_WIDTH (48)
#define SETTINGS_ICON_PADDING_OUT (4)
#define SETTINGS_ICON_PADDING_IN (4)
#define SETTINGS_X_POS \
  (ARROW_X_POS + 2 * ARROW_BOX_WIDTH + 2 * ARROW_ICON_PADDING_OUT)
#define SETTINGS_Y_POS (TOP_BAR_PADDING)
static int settings_open = 0;
void settings_button(Sound* click_sound) {
  DrawRectangle(SETTINGS_X_POS, SETTINGS_Y_POS, SETTINGS_WIDTH,
                TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING, ACCENT_COLOR2);
  if (BUTTON_RELEASED ==
      test_button(SETTINGS_X_POS, SETTINGS_Y_POS, SETTINGS_WIDTH,
                  TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING, MOUSE_BUTTON_LEFT)) {
    PlaySound(*click_sound);
    settings_open = !settings_open;
  }

  // the icon (all code below in the function) is CoPilot magic :d /wanders
  // sue me, i used AI 2x

  // New icon shape: a detailed gear
  int gear_radius = (SETTINGS_WIDTH - 2 * SETTINGS_ICON_PADDING_OUT) / 2;
  int gear_teeth = 12;  // More teeth for a detailed look
  int gear_inner_radius = gear_radius / 2;
  int gear_center_x = SETTINGS_X_POS + SETTINGS_WIDTH / 2;
  int gear_center_y =
      SETTINGS_Y_POS + (TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING) / 2;
  int tooth_depth = gear_radius / 4;  // Depth of each tooth

  for (int i = 0; i < gear_teeth; i++) {
    float angle = i * 2 * PI / gear_teeth;
    float next_angle = (i + 1) * 2 * PI / gear_teeth;
    float mid_angle = (angle + next_angle) / 2;

    Vector2 p1 = {gear_center_x + gear_radius * cos(angle),
                  gear_center_y + gear_radius * sin(angle)};
    Vector2 p2 = {gear_center_x + gear_radius * cos(next_angle),
                  gear_center_y + gear_radius * sin(next_angle)};
    Vector2 p3 = {
        gear_center_x + (gear_radius - tooth_depth) * cos(next_angle),
        gear_center_y + (gear_radius - tooth_depth) * sin(next_angle)};
    Vector2 p4 = {gear_center_x + (gear_radius - tooth_depth) * cos(mid_angle),
                  gear_center_y + (gear_radius - tooth_depth) * sin(mid_angle)};
    Vector2 p5 = {gear_center_x + (gear_radius - tooth_depth) * cos(angle),
                  gear_center_y + (gear_radius - tooth_depth) * sin(angle)};
    Vector2 p6 = {gear_center_x + gear_inner_radius * cos(angle),
                  gear_center_y + gear_inner_radius * sin(angle)};
    Vector2 p7 = {gear_center_x + gear_inner_radius * cos(next_angle),
                  gear_center_y + gear_inner_radius * sin(next_angle)};

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

// Defines for the function below
#define TAKE_BREAK_WIDTH (220)
#define TAKE_BREAK_PADDING_OUT (4)
#define TAKE_BREAK_PADDING_IN (4)
#define TAKE_BREAK_X_POS \
  (SETTINGS_X_POS + 2 * SETTINGS_WIDTH + 2 * SETTINGS_ICON_PADDING_OUT)
#define TAKE_BREAK_Y_POS (TOP_BAR_PADDING)

static int take_break_active = 0;

/**
 * This function adds the "Take a break" button to top bar.
 * It takes you to a different tab where you can forget your worries (tasks) and
 * watch a bird dance.
 */
void take_break_button(Sound* click_sound, Music *music) {
  DrawRectangle(TAKE_BREAK_X_POS, TAKE_BREAK_Y_POS, TAKE_BREAK_WIDTH,
                TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING, ACCENT_COLOR2);
  if (BUTTON_RELEASED ==
      test_button(TAKE_BREAK_X_POS, TAKE_BREAK_Y_POS, TAKE_BREAK_WIDTH,
                  TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING, MOUSE_BUTTON_LEFT) && !show_welcome_message) {
    PlaySound(*click_sound);
    take_break_active = !take_break_active;
    if (take_break_active) {
      ResumeMusicStream(*music);
    } else {
      PauseMusicStream(*music);
    }
  }
  char* take_break_text = "Take a break!";
  int take_break_font_size = 24;

  // None of this is responsive in any way, sorry
  DrawText(take_break_text, TAKE_BREAK_X_POS + take_break_font_size,
           TAKE_BREAK_Y_POS + take_break_font_size / 2, take_break_font_size,
           BG_COLOR1);
}

/**
 * This function draws the time label in the top bar.
 * It also allows the user to toggle between 24h and 12h formats by clicking on
 * the time label. The time label is updated every second.
 */
#define DATE_YEAR_LABEL_SIZE (48)
#define TIME_LABEL_SIZE (48)
#define TIME_LABEL_PERIOD_SIZE (24)
#define COLUMN_CHAR_PADDING (3)
void time_label(Sound* click_sound) {
  CLOCK_TIME now = get_current_clock_local();
  /**
   * as much as I love ternary one-liners, I had to change this line beause it
   * wasn't working for 12:00pm and 12:00 am as they were reversed
   *
   * char *time_period_str = app.use_24h_format == 1 ? "" : now.hour > 12 ? "
   * PM" : " AM";
   *
   */
  char* time_period_str;
  if (app.use_24h_format == 1) {
    time_period_str = "";  // No suffix in 24-hour format
  } else if (now.hour == 0) {
    time_period_str = " AM";
  } else if (now.hour == 12) {
    time_period_str = " PM";
  } else {
    time_period_str = now.hour > 12 ? " PM" : " AM";
  }

  char hour_str[3];    // Always remember to leave a slot for the '\n' character
  char minute_str[3];  // Always remember to leave a slot for the '\n' character
  snprintf(hour_str, 3, "%02d",
           app.use_24h_format == 0 && now.hour > 12 ? now.hour - 12 : now.hour);
  snprintf(minute_str, 3, "%02d", now.minute);
  int hour_str_length = MeasureText(&hour_str[0], TIME_LABEL_SIZE);
  int dot_str_length = MeasureText(":", TIME_LABEL_SIZE);
  int minute_str_length = MeasureText(&minute_str[0], TIME_LABEL_SIZE);
  int timer_period_str_length =
      MeasureText(time_period_str, TIME_LABEL_PERIOD_SIZE);

  if (BUTTON_RELEASED ==
      test_button(GetScreenWidth() - 2 * TOP_BAR_PADDING -
                      timer_period_str_length - minute_str_length -
                      dot_str_length - 2 * COLUMN_CHAR_PADDING -
                      hour_str_length,
                  TOP_BAR_HEIGHT - TOP_BAR_PADDING - TIME_LABEL_SIZE,
                  hour_str_length + dot_str_length + minute_str_length +
                      timer_period_str_length + 2 * COLUMN_CHAR_PADDING,
                  TIME_LABEL_SIZE, MOUSE_BUTTON_LEFT)) {
    app.use_24h_format = ++app.use_24h_format % 2;
    PlaySound(*click_sound);
  }

  DrawText(time_period_str,
           GetScreenWidth() - 2 * TOP_BAR_PADDING - timer_period_str_length,
           TOP_BAR_HEIGHT - 2 * TOP_BAR_PADDING - TIME_LABEL_PERIOD_SIZE,
           TIME_LABEL_PERIOD_SIZE, ACCENT_COLOR2);
  DrawText(&minute_str[0],
           GetScreenWidth() - 2 * TOP_BAR_PADDING - timer_period_str_length -
               minute_str_length,
           TOP_BAR_HEIGHT - TOP_BAR_PADDING - TIME_LABEL_SIZE, TIME_LABEL_SIZE,
           ACCENT_COLOR2);
  if (now.second % 2 == 0) {
    DrawText(":",
             GetScreenWidth() - 2 * TOP_BAR_PADDING - timer_period_str_length -
                 minute_str_length - dot_str_length - COLUMN_CHAR_PADDING,
             TOP_BAR_HEIGHT - TOP_BAR_PADDING - TIME_LABEL_SIZE,
             TIME_LABEL_SIZE, ACCENT_COLOR2);
  }
  DrawText(&hour_str[0],
           GetScreenWidth() - 2 * TOP_BAR_PADDING - timer_period_str_length -
               minute_str_length - dot_str_length - 2 * COLUMN_CHAR_PADDING -
               hour_str_length,
           TOP_BAR_HEIGHT - TOP_BAR_PADDING - TIME_LABEL_SIZE, TIME_LABEL_SIZE,
           ACCENT_COLOR2);
}

// The W and the 8 are (usually) the widest characters among letters and numbers
#define TEST_DATE_STR ("WWW 88ww")
// This should have been declared inside of the "day" file, but i don't know how
// to make it visible here if i do that - M37
const char* WEEKDAY_NAMES[] = {"Monday", "Tuesday",  "Wednesday", "Thursday",
                               "Friday", "Saturday", "Sunday"};
// Same thing for this one as the prev. array - M37
const char* MONTH_NAMES[] = {"Jan.", "Feb.", "Mar.",  "Apr.", "May",  "June",
                             "July", "Aug.", "Sept.", "Oct.", "Nov.", "Dec."};
// Full month names for use when app is maximized
const char* MONTH_NAMES_FULL[] = {
    "January", "February", "March",     "April",   "May",      "June",
    "July",    "August",   "September", "October", "November", "December"};

/**
 * This function draws the top bar of the app.
 * It contains the home, view, arrows, settings button and the time label.
 * It also contains the current month and year.
 */
void draw_top_bar(Sound* click_sound, Music *music) {
  DrawRectangle(0, 0, GetScreenWidth(), TOP_BAR_HEIGHT, ACCENT_COLOR1);
  home_button(click_sound);
  view_button(click_sound);
  arrows_buttons(click_sound);
  settings_button(click_sound);
  take_break_button(click_sound, music);
  time_label(click_sound);

  if (app.view_type == YEAR_VIEW) {
    char date_year_str[14];  // Copilot chose 14 :D
    snprintf(date_year_str, 14, "%d", app.view_first.date.year);
    DrawText(date_year_str,
             GetScreenWidth() / 2 -
                 MeasureText(date_year_str, DATE_YEAR_LABEL_SIZE) / 2,
             TOP_BAR_HEIGHT - TOP_BAR_PADDING - DATE_YEAR_LABEL_SIZE,
             DATE_YEAR_LABEL_SIZE, ACCENT_COLOR2);

    return;
  }

  DAY current = app.view_first;
  if (current.date.day > 3 * DAYS_IN_WEEK) {
    current.date.month += 1;
    validate_day(&current);
  }

  // Determine which month names array to use based on screen width
  const int min_width = 1000;
  const char** month_names =
      (GetScreenWidth() > min_width) ? MONTH_NAMES_FULL : MONTH_NAMES;

  // char date_year_str[14]; // Always remember to leave a slot for the '\n'
  // character snprintf(date_year_str, 14, "%s %d",
  // MONTH_NAMES[current.date.month - 1], current.date.year);

  char date_year_str[20];  // Adjusted size to accommodate longer month names
  snprintf(date_year_str, sizeof(date_year_str), "%s %d",
           month_names[current.date.month - 1], current.date.year);

  DrawText(date_year_str,
           GetScreenWidth() / 2 -
               MeasureText(date_year_str, DATE_YEAR_LABEL_SIZE) / 2,
           TOP_BAR_HEIGHT - TOP_BAR_PADDING - DATE_YEAR_LABEL_SIZE,
           DATE_YEAR_LABEL_SIZE, ACCENT_COLOR2);
}

// This piece of code is so cursed. Sorry for what i've created - M37
int find_ideal_text_size(char* text, int max_width) {
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

/**
 * This function draws the body of the app.
 * It contains the days of the week and the days of the month.
 * It also contains the schedule items for each day.
 */
#define BODY_PADDING_OUT (8)
#define BODY_PADDING_IN (2)
#define DATE_OUTLINE (4)
#define DATE_WIDTH (200)
#define DATE_PADDING (4)
#define MIN_DAY_MENU_WIDTH (80)
#define MIN_DAY_MENU_HEIGHT (40)
#define BUTTON_SIZE 20  // Small square button size

static int is_menu_visible = 0;
static Vector2 last_mouse_pos = {0};
static DAY last_pressed_day = {0};
static int mouse_on_title = 0;
static int is_today = 0;
static char title[SCHEDULE_TITLE_MAX_LEN + 1] = "\0";
static int title_letter_count = 0;
static char description[SCHEDULE_TEXT_MAX_LEN + 1] = "\0";
static int schedule_letter_count = 0;
static int wrap_title_index = 0;
static float key_hold_time = 0.0f;
const float hold_threshold = 0.5f;
const float delete_interval = 0.05f;

int text_shit_size;

void draw_body(DAY today) {
  int rows;
  if (app.view_type == WEEK_VIEW) {
    rows = 1;
  } else if (app.view_type == MONTH_VIEW) {
    rows = 6;
  } else {
    rows = 3;
  }

  int date_width = (GetScreenWidth() - 2 * BODY_PADDING_OUT -
                    (DAYS_IN_WEEK - 1) * BODY_PADDING_IN) /
                   DAYS_IN_WEEK;

  // TODO: Fix this idk
  int text_size = find_ideal_text_size(
      TEST_DATE_STR, date_width - 2 * DATE_OUTLINE - 2 * DATE_PADDING);
  text_shit_size = text_size;

  int week_bar_height = text_size + BODY_PADDING_IN;
  int date_height = (GetScreenHeight() - TOP_BAR_HEIGHT - week_bar_height -
                     2 * BODY_PADDING_OUT - (rows - 1) * BODY_PADDING_IN) /
                    rows;

  if (app.view_type != YEAR_VIEW) {
    for (int w = 0; w < DAYS_IN_WEEK; ++w) {
      // Draw weekday names
      DrawText(
          WEEKDAY_NAMES[w],
          BODY_PADDING_OUT + (date_width + BODY_PADDING_IN) * w + DATE_OUTLINE,
          TOP_BAR_HEIGHT + BODY_PADDING_OUT, text_size, BORDER_COLOR1);
    }
  }

  int is_day_of_current_month = (int)(app.view_type == WEEK_VIEW);
  DAY current = app.view_first;

  if (app.view_type != YEAR_VIEW) {
    for (int y = 0; y < rows; ++y) {
      for (int x = 0; x < DAYS_IN_WEEK; ++x) {
        if (app.view_type == MONTH_VIEW && is_day_of_current_month == 0 &&
            current.date.day == 1) {
          is_day_of_current_month = 1;
        }
        Color outline = BORDER_COLOR2;
        Color inner = BORDER_COLOR1;
        if (is_day_of_current_month == 1) {
          outline = BORDER_COLOR1;
          inner = BG_COLOR2;
        }
        // Override Color if drawing current day
        if (current.date.year == today.date.year &&
            current.date.month == today.date.month &&
            current.date.day == today.date.day) {
          outline = ACCENT_COLOR1;
        }

        char intstr[3];  // Always remember to leave a slot for the '\n'
                         // character
        snprintf(intstr, 3, "%d",
                 current.date.day);  // Like printf, but returns the string
                                     // instead of printing it (and also has a
                                     // max size for additional safety)

        // Draw date
        DrawRectangle(BODY_PADDING_OUT + (date_width + BODY_PADDING_IN) * x,
                      TOP_BAR_HEIGHT + BODY_PADDING_OUT + week_bar_height +
                          (date_height + BODY_PADDING_IN) * y,
                      date_width, date_height, outline);
        DrawRectangle(BODY_PADDING_OUT + (date_width + BODY_PADDING_IN) * x +
                          DATE_OUTLINE,
                      TOP_BAR_HEIGHT + BODY_PADDING_OUT + week_bar_height +
                          (date_height + BODY_PADDING_IN) * y + DATE_OUTLINE,
                      date_width - 2 * DATE_OUTLINE,
                      date_height - 2 * DATE_OUTLINE, inner);
        DrawText(&intstr[0],
                 BODY_PADDING_OUT + (date_width + BODY_PADDING_IN) * x +
                     DATE_OUTLINE + DATE_PADDING,
                 TOP_BAR_HEIGHT + BODY_PADDING_OUT + week_bar_height +
                     DATE_OUTLINE + DATE_PADDING +
                     (date_height + BODY_PADDING_IN) * y,
                 text_size, outline);

        // Draw squares equal to the number of schedule items for each day
        if (first_schedule_item(&current, 0) != -1) {
          int gap = 5;  // Gap between rectangles
          for (int i = 0; i < get_schedule_item_count(&current); ++i) {
            SCHEDULE_ITEM* item = get_schedule_item(&current, i);
            if (item != NULL) {
              DrawRectangle(
                  BODY_PADDING_OUT + (date_width + BODY_PADDING_IN) * x +
                      date_width - BUTTON_SIZE,
                  TOP_BAR_HEIGHT + BODY_PADDING_OUT + week_bar_height +
                      (date_height + BODY_PADDING_IN) * y +
                      (BUTTON_SIZE + gap) * i,
                  BUTTON_SIZE, BUTTON_SIZE, ACCENT_COLOR2);
            }
            // Break if the next rectangle would be outside the date box
            if ((BUTTON_SIZE + gap) * (i + 1) + BUTTON_SIZE > date_height) {
              break;
            }
          }
        }

        if ((BUTTON_RELEASED ==
             test_button(BODY_PADDING_OUT + (date_width + BODY_PADDING_IN) * x +
                             DATE_OUTLINE,
                         TOP_BAR_HEIGHT + BODY_PADDING_OUT + week_bar_height +
                             (date_height + BODY_PADDING_IN) * y + DATE_OUTLINE,
                         date_width - 2 * DATE_OUTLINE,
                         date_height - 2 * DATE_OUTLINE, MOUSE_BUTTON_LEFT))) {
          if (is_menu_visible == 0) {
            // printf("On %d date\n", current.date.day);
            is_menu_visible = 1;
            last_pressed_day = current;
            if (current.date.year == today.date.year &&
                current.date.month == today.date.month &&
                current.date.day == today.date.day) {
              is_today = 1;
            } else {
              is_today = 0;
            }
          } else if (is_menu_visible == 1 && !mouse_on_title) {
            is_menu_visible = 0;
          }
        }
        if ((BUTTON_RELEASED == test_button(0, 0, GetScreenWidth(),
                                            TOP_BAR_HEIGHT,
                                            MOUSE_BUTTON_LEFT))) {
          is_menu_visible = 0;
        }
        int prev_day = current.date.day;
        ++current.date.day;
        validate_day(&current);
        if (app.view_type == MONTH_VIEW && is_day_of_current_month == 1 &&
            prev_day > current.date.day) {
          is_day_of_current_month = 2;
        }
      }
    }
  } else {
    // Draw the year view
    int month_width = (GetScreenWidth() - 2 * BODY_PADDING_OUT -
                       (MONTHS_IN_YEAR / rows - 1) * BODY_PADDING_IN) /
                      (MONTHS_IN_YEAR / rows);
    int month_height = (GetScreenHeight() - TOP_BAR_HEIGHT -
                        2 * BODY_PADDING_OUT - (rows - 1) * BODY_PADDING_IN) /
                       rows;
    int month = 1;

    for (int y = 0; y < rows; ++y) {
      for (int x = 0; x < MONTHS_IN_YEAR / rows; ++x) {
        Color outline = BORDER_COLOR2;
        Color inner = BORDER_COLOR1;
        if (month == today.date.month && app.view_first.date.year == today.date.year) {
          outline = ACCENT_COLOR1;
        }
        // Draw month
        DrawRectangle(BODY_PADDING_OUT + (month_width + BODY_PADDING_IN) * x,
                      TOP_BAR_HEIGHT + BODY_PADDING_OUT +
                          (month_height + BODY_PADDING_IN) * y,
                      month_width, month_height, outline);
        DrawRectangle(BODY_PADDING_OUT + (month_width + BODY_PADDING_IN) * x +
                          DATE_OUTLINE,
                      TOP_BAR_HEIGHT + BODY_PADDING_OUT +
                          (month_height + BODY_PADDING_IN) * y + DATE_OUTLINE,
                      month_width - 2 * DATE_OUTLINE,
                      month_height - 2 * DATE_OUTLINE, inner);
        DrawText(MONTH_NAMES_FULL[month - 1],
                 BODY_PADDING_OUT + (month_width + BODY_PADDING_IN) * x +
                     DATE_OUTLINE + DATE_PADDING,
                 TOP_BAR_HEIGHT + BODY_PADDING_OUT + DATE_OUTLINE +
                     DATE_PADDING + (month_height + BODY_PADDING_IN) * y,
                 text_size, outline);
        if ((BUTTON_RELEASED ==
             test_button(BODY_PADDING_OUT +
                             (month_width + BODY_PADDING_IN) * x + DATE_OUTLINE,
                         TOP_BAR_HEIGHT + BODY_PADDING_OUT +
                             (month_height + BODY_PADDING_IN) * y +
                             DATE_OUTLINE,
                         month_width - 2 * DATE_OUTLINE,
                         month_height - 2 * DATE_OUTLINE, MOUSE_BUTTON_LEFT))) {
          app.view_first.date.month = month;
          app.view_type = MONTH_VIEW;
          snap_focus_to_1st_of_month();
        }
        ++month;
      }
    }
  }
}

/**
 * The code was making me go insane so I did some refactoring. You can think of
 * views like pages in a web app. Then thanks to some wacky shenanigans
 * happening in main, we can render them when we need them.
 *
 * - Mults
 */
//-------------------------------------VIEWS-------------------------------------//

void draw_settings(BUTTON* close_button) {
  // Define a window to render this view
  WINDOW settings_window = {
      .width = 300,
      .height = 200,
      .x = GetScreenWidth() / 2 - 300 / 2,
      .y = GetScreenHeight() / 2 - 200 / 2,
  };
  // Draw the window
  DrawRectangle(settings_window.x, settings_window.y, settings_window.width,
                settings_window.height, ACCENT_COLOR1);

  // Draw the close button
  draw_close_button(close_button, 20, settings_window);

  // Draw caption
  int caption_height = 30;
  int caption_x = settings_window.x + settings_window.width / 2;
  int caption_y = settings_window.y + 10;
  DrawText("Settings", caption_x - MeasureText("Settings", 20) / 2, caption_y,
           20, BG_COLOR1);

  // Checkboxes
  int checkbox_size = 20;
  // Implemented a function to draw the checkbox while refactoring
  BUTTON checkbox;
  // Draw the checkbox
  draw_checkbox(&checkbox, checkbox_size, settings_window, -30, "Clock format",
                app.use_24h_format);

  // Other test setting, feel free to use it for whatever you want
  // NOTE: This function is being called every frame, so using a boolean that
  // was declared in this scope, won't work because it will always be set to
  // default.
  int what = 0; // Oh I now understand the comment above - 13SHR
  BUTTON what_button;
  // boolean something; - WON'T WORK
  draw_checkbox(&what_button, checkbox_size, settings_window, 10, "What?",
                what);

  // Listen for click events, and toggle the 24-hour format
  if (BUTTON_RELEASED == test_button_struct(checkbox, MOUSE_BUTTON_LEFT)) {
    app.use_24h_format = !app.use_24h_format;
  }

  if (BUTTON_RELEASED == test_button_struct(what_button, MOUSE_BUTTON_LEFT)) {
    printf("What? pressed\n");
    what = (what + 1) % 2;
  }
}

/**
 * Who would use AI, am I right?
 *
 * Not me, I totally didn't ask Fred (ChatGPT) to make this function...
 *
 * - Mults
 *
 * NVM ChatGPT is dogshit, I did it myself...
 */
#define TIME_STRING_LENGTH 11  // HH:MM (a/p).m. + '/0'
int parse_time(char* time_str, CLOCK_TIME* output_time) {
  int hour;
  int minutes;
  int is_pm;
  int is_military = 1;

  if (strlen(time_str) == 0) {
    printf("Invalid time: Empty string\n");
    return -1; // Invalid time format
  }

  // Check for letters, if 'p' or 'A' are present, I'm gonna assume it's
  // military time This is not correct all the time but it will have to do for
  // now
  if (strstr(time_str, "p") || strstr(time_str, "P")) {
    is_military = 0;
    is_pm = 1;
  }
  if (strstr(time_str, "a") || strstr(time_str, "A")) {
    /* Checks if there are both A and P in the input time
       It should also check if there are several ones but... */
    if (!is_military) {
        printf("Invalid time: Both a and p\n");
        return -1;
    }
    is_military = 0;
    is_pm = 0;
  }

  // Iterate through all the characters,
  // if there are any other characters besides numbers, colons, space, p, or m,
  // return invalid
  for (int i = 0; i < strlen(time_str); i++) {
    if (time_str[i] != ' ' && time_str[i] != ':' && time_str[i] != 'p' &&
        time_str[i] != 'P' && time_str[i] != 'm' && time_str[i] != 'a' &&
        time_str[i] != 'A' && time_str[i] != '.' &&
        !(time_str[i] >= '0' && time_str[i] <= '9')) {

      printf("Invalid time: Invalid characters\n");
      return -1;  
    }
  }

  char* tok = strtok(strdup(time_str), ":");
  int token_counter = 1;

  while (tok) {
    if (token_counter > 2) {
      // For example if we have 23:32:65 -> invalid, we don't care about seconds
      // and stuff
      printf("Invalid time: To many colons\n");
      return -1;
    }

    // Check for hours
    if (token_counter == 1) {
      // Convert the token to a number, if it's not a valid hour, return -1
      hour = atoi(tok); // a or p alone is valid (it's a feature)

      if (!is_military && (hour < 0 || hour > 12)) {
        printf("Invalid time: AM/PM format with invalid hours\n");
        return -1;
      } else if (is_military && (hour < 0 || hour > 24)) {
        printf("Invalid time: Military format with invalid hours\n");
        return -1;
      }
    }
    // Check for minutes
    else if (token_counter == 2) {
      // Convert the token to a number, if it's not a valid hour, return -1
      minutes = atoi(tok);

      if (minutes < 0 || minutes >= 60) {
        printf("Invalid time: Invalid minutes\n");
        return -1;
      }
    }

    // Check for minutes
    tok = strtok(NULL, ":");
    token_counter++;
  }

  free(tok);
  if (!is_military) {
    if (is_pm && hour != 12) {
        hour += 12;  // Convert P.M. to 24-hour format
    } else if (!is_pm && hour == 12) {
        hour = 0;  // Midnight
    }
  }

  output_time->hour = hour;
  if (token_counter <= 2) {
    output_time->minute = 0;
  } else {
    output_time->minute = minutes;
  }
  output_time->second = 0;

  return 1;  // Return success if time is valid
}

int parse_duration(char* time_str, CLOCK_TIME* output_duration) {
  int hour;
  int minutes;

  if (strlen(time_str) == 0) {
    printf("Invalid duration: Empty input\n");
    return -1;
  }

  // Iterate through all the characters,
  // if there are any other characters besides numbers, colons, or spaces,
  // return invalid
  for (int i = 0; i < strlen(time_str); i++) {
    if (time_str[i] != ' ' && time_str[i] != ':' &&
        !(time_str[i] >= '0' && time_str[i] <= '9')) {
      return -1;  // Invalid time format
    }
  }

  char* tok = strtok(strdup(time_str), ":");
  int token_counter = 1;

  while (tok) {
    if (token_counter > 2) {
      // For example if we have 23:32:65 -> invalid, we don't care about seconds
      // and stuff
      return -1;
    }

    // Check for hours
    if (token_counter == 1) {
      // Conver the token to a number, if it's not a valid hour, return -1
      hour = atoi(tok);

      if (hour < 0 || hour > 24) {
        return -1;
      }
    }
    // Check for minutes
    else if (token_counter == 2) {
      // Conver the token to a number, if it's not a valid hour, return -1
      minutes = atoi(tok);

      if (minutes < 0 || minutes >= 60) {
        return -1;
      }
    }

    // Check for minutes
    tok = strtok(NULL, ":");
    token_counter++;
  }

  free(tok);

  output_duration->hour = hour;
  if (token_counter <= 2) {
    output_duration->minute = 0;
  } else {
    output_duration->minute = minutes;
  }
  output_duration->second = 0;

  return 1;  // Return success if time is valid
}

void monitor_user_input(int text_size, int title_box_width) {
  // TODO: Turn this into a function that monitors user input
  // Also, add a possibility to change the minutes here
  // Either that, or remove the time completely
  // Maybe I can either add a manual time, or no time at all so
  // it counts as a general task
  int key = GetCharPressed();
  while (key > 0) {
    // NOTE: Only allow keys in range [32..125]
    if ((key >= 32) && (key <= 125) &&
        (title_letter_count < SCHEDULE_TITLE_MAX_LEN)) {
      title[title_letter_count] = (char)key;
      title[title_letter_count + 1] = '\0';
      title_letter_count++;
      if (MeasureText(title, text_size) + text_size + 5 >
          ((int)title_box_width)) {
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
    if (wrap_title_index < 0) {
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
      if (wrap_title_index < 0) {
        wrap_title_index = 0;
      }
      title_letter_count--;
      if (title_letter_count < 0) {
        title_letter_count = 0;
      }
      title[title_letter_count] = '\0';
    }
  } else {
    key_hold_time = 0.0f;
  }

  if ((IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) && title_letter_count > 0) {
    /**
     * NOTE: The only idea I could think of was to make the user input the date
     * manually using ';' (semicolons) as separators. I'm not a UX developer.
     *
     * - Mults
     */

    SCHEDULE_ITEM item = {0};
    // Process title to get start time and duration
    char* data[3];
    data[0] = "\0";
    data[1] = "\0";
    data[2] = "\0";

    char* buff = title;
    buff = strtok(buff, ";");
    int i = 0;
    while (buff) {
      printf("Buff: %s\n", buff);
      data[i++] = buff;
      buff = strtok(NULL, ";");
    }

    if (data[0]) {
      printf("Task: %s\n", data[0]);
      strcpy(item._title, data[0]);
    }

    CLOCK_TIME output_time;
    CLOCK_TIME output_duration;

    if (data[1]) {
      printf("Start: %s\n", data[1]);
      if (parse_time(data[1], &output_time) == -1) {
        // Invalid time, start time is 'now'
        item.begin_time = get_current_clock_local();  // temporary
        printf("Invalid time: Replaced by current time\n");
      } else {
        // Valid time
        item.begin_time = output_time;
        printf("Valid time\n");
      }
    } else {
      // Start time is 'now'
      item.begin_time = get_current_clock_local();  // temporary
      printf("Invalid time : No data replaced by current time\n");
    }

    if (data[2]) {
      printf("Duration: %s\n", data[2]);
      if (parse_duration(data[2], &output_duration) == -1) {
        printf("Invalid duration: using default duration\n");
        // Default duration of 1 hour
        item.duration.hours = 1;
        item.duration.minutes = 0;
      } else {
        printf("Valid duration\n");
        item.duration.hours = output_duration.hour;
        item.duration.minutes = output_duration.minute;
      }
    } else {
      printf("No duration data: using default duration\n");
      // Default duration of 1 hour
      item.duration.hours = 1;
      item.duration.minutes = 0;
    }

    strcpy(item._description, description);

    // + randomize a few minutes. otherwise, all items will be on the same time,
    // and that doesn't work
    /**
     * item.begin_time.minute += GetRandomValue(0, 59); // also, temporary :D
     * item.duration.hours = 1; // temporary
     * item.duration.minutes = 0; // temporary
     *
     * The temporary things were removed - Mults :)
     */

    add_schedule_item(&last_pressed_day, item);

    title[0] = '\0';
    description[0] = '\0';
    title_letter_count = 0;
    schedule_letter_count = 0;
    wrap_title_index = 0;
  }
}

#define SCROLL_PADDING_BOTTOM (10)
#define SCROLL_PADDING_BETWEEN (10)
#define SCROLL_BUTTON_WIDTH_RATIO (10)
static int scroll_index = 0;

// Draws the arrows in the menu_window to scroll between the items 
void arrows_scroll(WINDOW menu_window, int nb_items) {
    int button_width = menu_window.width / SCROLL_BUTTON_WIDTH_RATIO;

    // Don't display up arrow if the top item is the first one
    if (scroll_index) {
        Rectangle arrow_up = {
            menu_window.x + menu_window.width / 2. - button_width - SCROLL_PADDING_BETWEEN / 2.,
            menu_window.y + menu_window.height * 6 / 7.,
            button_width,
            menu_window.height / 7. - SCROLL_PADDING_BOTTOM};
        DrawRectangleRec(arrow_up, ACCENT_COLOR2);

        // Detect if the button is clicked
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                CheckCollisionPointRec(GetMousePosition(), arrow_up)) {
            scroll_index--;
        }

        // The arrow itself
        Vector2 v1;
        v1.x = arrow_up.x + arrow_up.width / 2.;
        v1.y = arrow_up.y + arrow_up.height / 5.;
        Vector2 v2;
        v2.x = arrow_up.x + arrow_up.width / 5.;
        v2.y = arrow_up.y + arrow_up.height * 4 / 5.;
        Vector2 v3;
        v3.x = arrow_up.x + arrow_up.width * 4 / 5.;
        v3.y = arrow_up.y + arrow_up.height * 4 / 5.;
        DrawTriangle(v1, v2, v3, BG_COLOR1);
        
    }

    // Don't display down arrow if the bottom item is the last one
    if (scroll_index < nb_items - 3) {
        Rectangle arrow_down = {
            menu_window.x + menu_window.width / 2. + SCROLL_PADDING_BETWEEN / 2.,
            menu_window.y + menu_window.height * 6 / 7.,
            button_width,
            menu_window.height / 7. - SCROLL_PADDING_BOTTOM};
        DrawRectangleRec(arrow_down, ACCENT_COLOR2);

        // Detect if the button is clicked
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                CheckCollisionPointRec(GetMousePosition(), arrow_down)) {
            scroll_index++;
        }

        // The arrow itself
        Vector2 v1;
        v1.x = arrow_down.x + arrow_down.width / 2.;
        v1.y = arrow_down.y + arrow_down.height * 4 / 5.;
        Vector2 v2;
        v2.x = arrow_down.x + arrow_down.width / 5.;
        v2.y = arrow_down.y + arrow_down.height / 5.; 
        Vector2 v3;
        v3.x = arrow_down.x + arrow_down.width * 4 / 5.;
        v3.y = arrow_down.y + arrow_down.height / 5.;
        printf("%f %f\n", arrow_down.y, v1.y);
        printf("%f %f\n", arrow_down.y, v2.y);
        printf("%f %f\n", arrow_down.y, v3.y);
        DrawTriangle(v3, v2, v1, BG_COLOR1);
    }
}

void draw_menu(int text_size, BUTTON* close_button) {
  //-------------------------------------DRAW MENU
  // WINDOW-------------------------------------//
  WINDOW menu_window = {.x = GetScreenWidth() / 4,
                        .y = (GetScreenHeight() + 2 * TOP_BAR_HEIGHT) / 4,
                        .width = GetScreenWidth() / 2,
                        .height = GetScreenHeight() / 2};
  // Draw Schedule Box
  DrawRectangle(menu_window.x, menu_window.y, menu_window.width,
                menu_window.height, ACCENT_COLOR1);

  char str[40];
  snprintf(str, 40, "Task for %s",
           is_today ? "Today"
                    : TextFormat("%d/%d/%d", last_pressed_day.date.day,
                                 last_pressed_day.date.month,
                                 last_pressed_day.date.year));

  // Draw a close button
  draw_close_button(close_button, 15, menu_window);

  /**
   * Center a object(horizontal or vertical) in a given rectangle box
   *  x = X + (W-w)/2
   *  x -> centre position , X-> position of rectangle , W = width of rectangle
   * , w = width of object
   */
  int text_pos = (GetScreenWidth() / 4) +
                 (GetScreenWidth() / 2 - MeasureText(str, text_size)) / 2;

  DrawText(str, text_pos, (GetScreenHeight() + 2 * TOP_BAR_HEIGHT) / 4 + 10,
           text_size, BG_COLOR2);

  // I'm using wolle's function instead, even though it wasn't meant for this
  // purpose draw_centered_text(str, menu_window.x, menu_window.y - 180,
  // menu_window.width, menu_window.height, text_size, BG_COLOR1); nvm, it
  // didn't work on windows lol - Mults

  // Day Schedule Title Box
  Rectangle title_box = {(float)GetScreenWidth() / 4 + 20,
                         (float)(GetScreenHeight() + 2 * TOP_BAR_HEIGHT) / 4 +
                             (float)GetScreenHeight() / 12,
                         (float)GetScreenWidth() / 2 - 40,
                         (float)GetScreenHeight() / 14};
  // Rectangle dit_details_box = {
  //             .x = GetScreenWidth() / 4 + 20,
  //             .y = GetScreenHeight()+ 2*TOP_BAR_HEIGHT/4+GetScreenHeight()/12
  //           };

  DrawRectangleRec(title_box, BORDER_COLOR1);

  Vector2 title_pos = {
      .x = (float)(GetScreenWidth()) / 4 + 20 +
           (((float)GetScreenWidth() / 2) - 40 -
            (MeasureText("Task;HH:MM (AM/PM/24H);Duration", text_size))) /
               2,
      .y = (float)(GetScreenHeight() + 2 * TOP_BAR_HEIGHT) / 4 +
           (float)GetScreenHeight() / 12 +
           ((float)GetScreenHeight() / 12 - text_size) / 2};
  //-------------------------------------DRAW MENU
  // WINDOW-------------------------------------//

  //-------------------------------------CHECK USER
  // INPUT-------------------------------------//
  // Only draw text if there is not any input from the user yet.
  if (title_letter_count == 0) {
    DrawText("Task;HH:MM (am/pm/24h);Duration", title_pos.x, title_pos.y, text_size,
             ColorAlpha(BG_COLOR2, 0.6));
  }

  if (CheckCollisionPointRec(GetMousePosition(), title_box))
    mouse_on_title = 1;
  else
    mouse_on_title = 0;

  if (mouse_on_title) {
    SetMouseCursor(MOUSE_CURSOR_IBEAM);
    monitor_user_input(text_size, title_box.width);
  } else {
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
  }
  //-------------------------------------CHECK USER
  // INPUT-------------------------------------//

  /**
   * Basically, all in this 'if-statement' checks if there are any items in the
   * schedule, and draws them, if you have the menu open. Yes, is a mess.
   *
   * CoPilot magic used for the coordinates.
   * sue me, i used AI 3x
   *
   * /wanders
   *
   * =================================================
   * Added a delete button for each item,
   * calls remove_schedule_item() when clicked.
   *
   * FIXME: Can add more than 3 items, but only 3 are displayed.
   *        The rest are stored in the linked list (day.c) and will show up when
   * an item is deleted. ALSO, the mouse click to delete an item closes the
   * menu.. had a lot of trouble with these inputs that perpetuate through
   * frames and just couldn't fix it ://
   *
   * Still a mess, good luck :)
   *
   * /Flameo(Flam30)
   * 
   * =================================================
   * I added the scroll functionnality, by modifying
   * a little bit the way we play with the NODE struct.
   * The input-through-frames makes it horrible to use though.
   * - 13SHR
   */

  if (is_day_in_list(&last_pressed_day) && !is_day_empty(&last_pressed_day)) {
    SCHEDULE_ITEM *first_item, *second_item, *third_item;
    int first_index, second_index, third_index = -1;
    first_index = first_schedule_item(&last_pressed_day, scroll_index);
    first_item = get_schedule_item(&last_pressed_day, first_index);

    second_index =
        has_next_schedule_item(&last_pressed_day, first_index);
    if (second_index != -1) {
      second_item = get_schedule_item(&last_pressed_day, second_index);
      third_index =
          has_next_schedule_item(&last_pressed_day, second_index);
      if (third_index != -1) {
        third_item = get_schedule_item(&last_pressed_day, third_index);
      }
    }

    // Calculate the y position for the items based on the title box
    float item_start_y =
        title_box.y + title_box.height + 10;  // 10 pixels padding
    float button_size = 20;                   // Small square button size

    // Draw the first item
    DrawRectangleRec((Rectangle){.x = title_box.x,
                                 .y = item_start_y,
                                 .width = title_box.width,
                                 .height = title_box.height},
                     BORDER_COLOR1);
    draw_centered_text(first_item->_title, title_box.x, item_start_y,
                       title_box.width, title_box.height / 2, text_size,
                       BG_COLOR2);

    // Calculate the position for the delete button in the top right corner of
    // the first item
    Rectangle delete_button_first = {
        title_box.x + title_box.width - BUTTON_SIZE - 5,  // X position
        item_start_y + 5,  // Y position (5 pixels from the top)
        BUTTON_SIZE, BUTTON_SIZE};

    // Draw the delete button
    DrawRectangleRec(delete_button_first, RED);  // Draw delete button in red

    // Detect if the delete button for the first item is clicked
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
            CheckCollisionPointRec(GetMousePosition(), delete_button_first)) {
        remove_schedule_item(&last_pressed_day,
                           first_index);  // Delete the first item
        scroll_index = 0;
    }

    // Draw the starting time and duration for the first item
    char start_time_str[6], duration_str[10];
    format_time(first_item->begin_time, start_time_str);
    sprintf(duration_str, "%02d:%02d", first_item->duration.hours,
            first_item->duration.minutes);
    char time_duration_str[40];
    sprintf(time_duration_str, "Start: %s, Duration: %s", start_time_str,
            duration_str);
    draw_centered_text(time_duration_str, title_box.x,
                       item_start_y + title_box.height / 2, title_box.width,
                       title_box.height / 2, text_size, BG_COLOR1);

    // Draw the second item, if it exists
    if (second_index != -1) {
      float second_item_y =
          item_start_y + title_box.height + 10;  // Position for the second item
      DrawRectangleRec((Rectangle){.x = title_box.x,
                                   .y = second_item_y,
                                   .width = title_box.width,
                                   .height = title_box.height},
                       BORDER_COLOR1);
      draw_centered_text(second_item->_title, title_box.x, second_item_y,
                         title_box.width, title_box.height / 2, text_size,
                         BG_COLOR2);

      // Calculate the position for the delete button in the top right corner of
      // the item
      Rectangle delete_button_second = {
          title_box.x + title_box.width - BUTTON_SIZE - 5,  // X position
          second_item_y + 5,  // Y position (5 pixels from the top)
          BUTTON_SIZE, BUTTON_SIZE};

      // Draw the delete button
      DrawRectangleRec(delete_button_second, RED);

      // Detect click on the second item's delete button
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
          CheckCollisionPointRec(GetMousePosition(), delete_button_second)) {
        remove_schedule_item(&last_pressed_day,
                             second_index);  // Delete the second item
      }

      // Draw the starting time and duration for the second item
      format_time(second_item->begin_time, start_time_str);
      sprintf(duration_str, "%02d:%02d", second_item->duration.hours,
              second_item->duration.minutes);
      sprintf(time_duration_str, "Start: %s, Duration: %s", start_time_str,
              duration_str);
      draw_centered_text(time_duration_str, title_box.x,
                         second_item_y + title_box.height / 2, title_box.width,
                         title_box.height / 2, text_size, BG_COLOR1);
    }

    // Draw the third item, if it exists
    if (third_index != -1) {
      float third_item_y =
          item_start_y +
          2 * (title_box.height + 10);  // Position for the third item
      DrawRectangleRec((Rectangle){.x = title_box.x,
                                   .y = third_item_y,
                                   .width = title_box.width,
                                   .height = title_box.height},
                       BORDER_COLOR1);
      draw_centered_text(third_item->_title, title_box.x, third_item_y,
                         title_box.width, title_box.height / 2, text_size,
                         BG_COLOR2);

      // Calculate the position for the delete button in the top right corner of
      // the item
      Rectangle delete_button_third = {
          title_box.x + title_box.width - BUTTON_SIZE - 5,  // X position
          third_item_y + 5,  // Y position (5 pixels from the top)
          BUTTON_SIZE, BUTTON_SIZE};

      // Draw the delete button
      DrawRectangleRec(delete_button_third, RED);

      // Detect click on the third item's delete button
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
          CheckCollisionPointRec(GetMousePosition(), delete_button_third)) {
        remove_schedule_item(&last_pressed_day,
                             third_index);  // Delete the third item
      }

      // Draw the starting time and duration for the third item
      format_time(third_item->begin_time, start_time_str);
      sprintf(duration_str, "%02d:%02d", third_item->duration.hours,
              third_item->duration.minutes);
      sprintf(time_duration_str, "Start: %s, Duration: %s", start_time_str,
              duration_str);
      draw_centered_text(time_duration_str, title_box.x,
                         third_item_y + title_box.height / 2, title_box.width,
                         title_box.height / 2, text_size, BG_COLOR1);
    }

    // Draw the scroll arrows if necessary
    int nb_items = get_schedule_item_count(&last_pressed_day);
    
    if (nb_items > 3) {
        arrows_scroll(menu_window, nb_items);
    }

  }

  DrawText(&title[wrap_title_index], title_box.x + 5, title_pos.y, text_size,
           ACCENT_COLOR2);

  if (mouse_on_title) {
    CLOCK_TIME now = get_current_clock_local();
    if (title_letter_count < SCHEDULE_TITLE_MAX_LEN) {
      if (now.second % 2 == 0) {
        if (wrap_title_index > 0) {
          DrawText("_", title_box.width + title_box.x - text_size,
                   title_pos.y + text_size / 2., text_size,
                   ACCENT_COLOR2);
        } else {
          DrawText("_", title_box.x + 10 + MeasureText(title, text_size),
                   title_pos.y, text_size,
                   ACCENT_COLOR2);
        }
      }
    }
  }
}

void draw_parrot(Texture2D tex_parrot, BUTTON* close_button) {
  // Define a window
  WINDOW gif_window = {
      .width = 600,
      .height = 600,
      .x = (GetScreenWidth() - 600) / 2,
      .y = (GetScreenHeight() - 600) / 2,
  };
  // Draw the window
  DrawRectangle(gif_window.x, gif_window.y, gif_window.width, gif_window.height,
                BLUE);

  // Make a close button - same as below just bigger
  draw_close_button(close_button, 40, gif_window);

  // Draw animation
  DrawTexture(tex_parrot, (GetScreenWidth() - tex_parrot.width) / 2,
              (GetScreenHeight() - tex_parrot.height) / 2, WHITE);
}

/*This function will choose a funny welcome text from the welcome.txt file*/
char* get_welcome_text(char* filename) {
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    return "Axolotls are cool animals!";  // placeholder in case the file is not
                                          // found
  }

  // read the lines of the text file
  int lines = 0;
  char buffer[256];
  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    lines++;
  }

  // choose a random line
  srand(time(NULL));
  int random_line = rand() % lines;
  rewind(file);  // go to the beginning of the file

  // get the chosen line
  char* welcome_text;

  for (int current_line = 0; current_line < random_line; current_line++) {
    fgets(buffer, sizeof(buffer), file);
  }
  welcome_text = strdup(buffer);

  fclose(file);
  return welcome_text;
}

/* an initial message for the user :3
NOTE: it has a small bug where you will press the date that is under the welcome
window but i REALLY cant be fucked*/
void welcome_message(char* message, BUTTON *close_button) {
  // i have no idea how raylib works so im just copying what y'all did
  WINDOW welcome_window = {
      .x = GetScreenWidth() / 4,
      .y = GetScreenHeight() / 2 - GetScreenHeight() / 16,
      .width = GetScreenWidth() / 2,
      .height = GetScreenHeight() / 8,
  };

  // Draw the window
  DrawRectangle(welcome_window.x, welcome_window.y, welcome_window.width,
                welcome_window.height, ACCENT_COLOR1);

  /* Draw the close button in the bottom center of the window
     NOTE: Adding text to a button is way too complicated for what it should be
     so im not doing it, red should be enogh 
    ------------
    It was not that difficult! - 13SHR */
  Rectangle close_rectangle = {
      welcome_window.x +
          (welcome_window.width - 100) / 2.,           // Center horizontally
      welcome_window.y + welcome_window.height - 60,      // Position at the bottom
      100,                                            // Button width
      50                                             // Button height
  };
  close_button->size = 75;
  close_button->x = close_rectangle.x;
  close_button->y = close_rectangle.y;
  DrawRectangleRec(close_rectangle, ACCENT_COLOR2);
  draw_centered_text("Cool!", 
     close_rectangle.x, 
     close_rectangle.y,
     close_rectangle.width,
     close_rectangle.height, 
     20, 
     BG_COLOR1);

  draw_centered_text("Welcome to hacktoberfest! Here is a fun fact for you: ",
           welcome_window.x, welcome_window.y, welcome_window.width, welcome_window.height / 4., 20, BG_COLOR1);
  DrawText(message, welcome_window.x + 50, welcome_window.y + 70, 20, BG_COLOR1);
}

//-------------------------------------VIEWS-------------------------------------//

#define WIN_MIN_WIDTH (800)
#define WIN_MIN_HEIGHT (400)

int main(int argc, char** argv) {
  // Surprise
  if (argc == 2 && strcmp(argv[1], "--surprise") == 0) {
#ifdef _WIN32
    char* file_path = "..\\assets\\surprise.txt";
#else
    char* file_path = "../assets/surprise.txt";
#endif

    printArt(file_path);

    return 0;
  }

  // Regularly scheduled programming
  (void)argc;
  (void)argv;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(WIN_MIN_WIDTH, WIN_MIN_HEIGHT, "Hacktoberfest");
  MaximizeWindow();  // No time to make it responsive
  SetWindowMinSize(WIN_MIN_WIDTH, WIN_MIN_HEIGHT);
  InitAudioDevice();
  SetTargetFPS(60);

  /**
   * I'm sure there is a better way to optimize the memory usage for this, but
   * currently, I am not aware of such way I'm sorry for wasting your memory for
   * such a simple task - M37
   *
   * -----------
   *
   * Memory is there to be wasted - Mults
   *
   * Update: The I made some helper functions to make the parsing easier.
   * I didn't feel like doing dynamic allocation for now.
   */
  //------------------------------LOAD ASSETS------------------------------//
  char local_path[strlen(argv[0]) + 1];  // Always remember to leave a slot for the '\0' character
  strcpy(local_path, argv[0]);
  truncate_str_after_directory_separator(local_path);

  char load_buffer[512];  // Large enough buffer to store the paths, this will
                          // be reused because idkf, fuck malloc

  // Load click sound
  parse_asset_path(load_buffer, local_path, "click.wav");
  Sound click_sound = LoadSound(load_buffer);

  // Load Crab Rave... the file was here so I used it - 13SHR
  parse_asset_path(load_buffer, local_path, "crab_rave.wav");
  Music crab_rave = LoadMusicStream(load_buffer);
  crab_rave.looping = true;
  PlayMusicStream(crab_rave);
  PauseMusicStream(crab_rave);

  // Load parrot gif
  parse_asset_path(load_buffer, local_path, "party_parrot.gif");
  int animation_frames = 0;
  Image img_party_parrot = LoadImageAnim(load_buffer, &animation_frames);
  Texture2D tex_party_parrot = LoadTextureFromImage(img_party_parrot);

  // Load welcome text
  parse_asset_path(load_buffer, local_path, "welcome.txt");
  char* welcome_text = get_welcome_text(load_buffer);

  //------------------------------LOAD ASSETS------------------------------//

  //------------------------------APP CONFIG-------------------------------//
  app.use_24h_format = 1;

  DAY today = get_today_local();
  printf("Today: %d/%d/%d\n", today.date.day, today.date.month,
         today.date.year);

  app.view_first = today;
  // NOTE: Call snap_focus_to_1st_of_month() if setting the starting view_type
  // as MONTH_VIEW
  if (app.view_first.weekday == 0) {  // Assuming 0 represents Sunday, IDFK KNOW
                                      // WHY IT WORKS EVEN THOUGH MONDAY IS 0
    app.view_first.date.day -= 6;
  } else {
    app.view_first.date.day -= (int)(app.view_first.weekday) - 1;
  }
  validate_day(&app.view_first);
  printf("First Monday: %d/%d/%d\n", app.view_first.date.day,
         app.view_first.date.month, app.view_first.date.year);

  //-------------------------------APP CONFIG------------------------------//

  // Code for GIF
  // Example followed from:
  // https://www.raylib.com/examples/textures/loader.html?name=textures_gif_player
  // ------------------------------------------------------------------------
  unsigned int nextFrameDataOffset = 0;

  int current_anim_frame = 0;
  int frameDelay = 3;  // Smaller number, faster switching between frames
  int frameCounter = 0;
  // ------------------------------------------------------------------------
  // End of code for GIF

  //----------------------------MAIN RENDER LOOP---------------------------//
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BG_COLOR1);
    draw_top_bar(&click_sound, &crab_rave);
    draw_body(today);

    // welcome window
    if (show_welcome_message) {
      BUTTON close_button;
      welcome_message(welcome_text, &close_button);
      // LISTEN FOR BUTTON EVENTS
      if (BUTTON_RELEASED ==
          test_button_struct(close_button, MOUSE_BUTTON_LEFT)) {
        show_welcome_message = 0;
      }
      // Do not open the task menu if the close button is pressed
      if (BUTTON_RELEASED ==
          test_button_struct(close_button, MOUSE_BUTTON_LEFT)) {
        is_menu_visible = 0;
      }
    } else if (take_break_active) {
      // Go one step further in the music (yes raylib has a weird way to manage music)
      UpdateMusicStream(crab_rave);
      // Update the bird gif so it renders properly whenever this is active
      frameCounter++;
      // Get next frame of gif once delay has finished
      if (frameCounter >= frameDelay) {
        current_anim_frame++;
        // If animation reaches end, reset current frame to beginning
        if (current_anim_frame >= animation_frames) current_anim_frame = 0;

        nextFrameDataOffset = img_party_parrot.width * img_party_parrot.height *
                              4 * current_anim_frame;
        UpdateTexture(
            tex_party_parrot,
            ((unsigned char*)img_party_parrot.data) + nextFrameDataOffset);

        // Reset the delay counter
        frameCounter = 0;
      }

      // Draw the corresponding frame of the gif
      BUTTON close_button;
      draw_parrot(tex_party_parrot, &close_button);

      // LISTEN FOR BUTTON EVENTS
      if (BUTTON_RELEASED ==
          test_button_struct(close_button, MOUSE_BUTTON_LEFT)) {
        take_break_active = 0;
      }
      // Do not open the task menu if the close button is pressed
      if (BUTTON_RELEASED ==
          test_button_struct(close_button, MOUSE_BUTTON_LEFT)) {
        is_menu_visible = 0;
      }
    } else if (settings_open) {
      BUTTON close_button;
      draw_settings(&close_button);

      // LISTEN FOR BUTTON EVENTS
      if (BUTTON_RELEASED ==
          test_button_struct(close_button, MOUSE_BUTTON_LEFT)) {
        settings_open = 0;
      }
      // Do not open the task menu if the close button is pressed
      if (BUTTON_RELEASED ==
          test_button_struct(close_button, MOUSE_BUTTON_LEFT)) {
        is_menu_visible = 0;
      }
    } else if (is_menu_visible) {
      BUTTON close_button;
      draw_menu(text_shit_size, &close_button);

      // LISTEN FOR BUTTON EVENTS
      if (BUTTON_RELEASED ==
          test_button_struct(close_button, MOUSE_BUTTON_LEFT)) {
        is_menu_visible = 0;
      }
    }

    EndDrawing();
  }
  //----------------------------MAIN RENDER LOOP---------------------------//

  //----------------------------FREE RESOURCES-----------------------------//
  free(welcome_text);
  UnloadTexture(tex_party_parrot);
  UnloadImage(img_party_parrot);
  UnloadSound(click_sound);
  StopMusicStream(crab_rave);
  UnloadMusicStream(crab_rave);
  CloseAudioDevice();
  CloseWindow();
  //----------------------------FREE RESOURCES-----------------------------//

  return 0;
}
