#include "time.h" // time_t, time, struct tm, gmtime

#include "day.h"

DAY get_today_local(void) {
    DAY ret = {0};

    time_t raw_time;
    struct tm *timespec;

    time(&raw_time);
    timespec = localtime(&raw_time);

    ret.date.day = timespec->tm_mday;
    ret.date.month = timespec->tm_mon + 1; // Month since january + 1
    ret.date.year = timespec->tm_year + 1900; // Years since 1900 + 1900

    // No need for a switch here, it's just a simple explicit conversion - M37
    /* switch (timespec->tm_wday) {
        case 0:
            ret.weekday = SUNDAY;
            break;
        case 1:
            ret.weekday = MONDAY;
            break;
        case 2:
            ret.weekday = TUESDAY;
            break;
        case 3:
            ret.weekday = WEDNESDAY;
            break;
        case 4:
            ret.weekday = THURSDAY;
            break;
        case 5:
            ret.weekday = FRIDAY;
            break;
        case 6:
            ret.weekday = SATURDAY;
            break;
    } */
    ret.weekday = (WEEKDAY)((int)(timespec->tm_wday));

    return ret;
}

CLOCK_TIME get_current_clock_local(void) {
    CLOCK_TIME ret = {0};

    time_t raw_time;
    struct tm *timespec;

    time(&raw_time);
    timespec = localtime(&raw_time);
    
    ret.hour = timespec->tm_hour;
    ret.minute = timespec->tm_min;
    ret.second = timespec->tm_sec;

    return ret;
}

