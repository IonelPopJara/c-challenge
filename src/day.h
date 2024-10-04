#ifndef DAY_H
#define DAY_H

#include "dyn_array.h"

// Have fun serializing this (I'm so sorry again)

#define SCHEDULE_TITLE_MAX_LEN (64)
#define SCHEDULE_TEXT_MAX_LEN (1024)
#define DAY_MAX_SCHEDULE_ITEMS (128)

#define DAYS_IN_WEEK (7)
#define WEEKS_IN_MONTH (7)

typedef enum weekday {
    MONDAY, // I'll begin with Monday. Sue me.
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
} WEEKDAY;

typedef struct clock_time {
    int hour;
    int minute;
    int second;
} CLOCK_TIME;

typedef struct schedule_item {
    CLOCK_TIME begin_time;
    struct duration {
        int hours;
        int minutes;
    } duration;
    char title[SCHEDULE_TITLE_MAX_LEN];
    char description[SCHEDULE_TEXT_MAX_LEN];
} SCHEDULE_ITEM;

typedef struct day {
    struct date {
        int month;
        int day;
        int year;
    } date;
    WEEKDAY weekday;
} DAY;

DAY get_today_local(void);
CLOCK_TIME get_current_clock_local(void);

#endif // DAY_H

