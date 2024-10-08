#include "time.h" // time_t, time, struct tm, gmtime

#include "day.h"

void validate_day(DAY *value) {
    time_t raw_time;
    struct tm *timespec;
    
    time(&raw_time);
    timespec = localtime(&raw_time);
    
    timespec->tm_mday = value->date.day;
    timespec->tm_mon = value->date.month - 1;
    timespec->tm_year = value->date.year - 1900;
    
    mktime(timespec);
    
    value->date.day = timespec->tm_mday;
    value->date.month = timespec->tm_mon + 1; // Month since january + 1
    value->date.year = timespec->tm_year + 1900; // Years since 1900 + 1900
    value->weekday = (WEEKDAY)((int)(timespec->tm_wday));
}

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

/**
 * Let god never witness this behemoth below
 * 
 * basically, a way to find days to schedule items to draw them when the menu is open
 * 
 * (yes, its all linear, what you gonna do about it?)
 * (some1 probably should do something about it)
 * 
 * /wanders
 */

typedef struct node {
    unsigned int key; // key for the day, example 01-02-2021 is 20210201
    SCHEDULE_ITEM value[DAY_MAX_SCHEDULE_ITEMS];
    int schedule_item_count;
    struct node *next;
} NODE;

NODE *head = NULL;

unsigned int get_key(DAY day) {
    return day.date.day + day.date.month * 100 + day.date.year * 10000;
}

int is_day_in_list(DAY *day) {
    unsigned int key = get_key(*day);
    NODE *current = head;
    while (current != NULL) {
        if (current->key == key) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void add_schedule_item(DAY *day, SCHEDULE_ITEM item) {
    unsigned int key = get_key(*day);
    NODE *current = head;
    while (current != NULL) {
        if (current->key == key) {
            current->value[current->schedule_item_count] = item;
            current->schedule_item_count++;
            return;
        }
        current = current->next;
    }
    NODE *new_node = malloc(sizeof(NODE));
    new_node->key = key;
    new_node->value[0] = item;
    new_node->schedule_item_count = 1;
    new_node->next = head;
    head = new_node;
}

void remove_schedule_item(DAY *day, int index) {
    unsigned int key = get_key(*day);
    NODE *current = head;
    while (current != NULL) {
        if (current->key == key) {
            for (int i = index; i < current->schedule_item_count - 1; ++i) {
                current->value[i] = current->value[i + 1];
            }
            current->schedule_item_count--;
            return;
        }
        current = current->next;
    }
}

int is_day_empty(DAY *day) {
    unsigned int key = get_key(*day);
    NODE *current = head;
    while (current != NULL) {
        if (current->key == key) {
            return current->schedule_item_count == 0;
        }
        current = current->next;
    }
    return 1;
}

/**
 * @return the index of the first schedule item of the day, or -1 if there are none
 */
int first_schedule_item(DAY *day) {
    unsigned int key = get_key(*day);
    NODE *current = head;
    int first_index = -1;
    SCHEDULE_ITEM ret = {0};
    ret.begin_time.hour = 25; // later than any possible hour
    while (current != NULL) {
        if (current->key == key) {
            for (int i = 0; i < current->schedule_item_count; ++i) {
                if (current->value[i].begin_time.hour < ret.begin_time.hour) {
                    ret = current->value[i];
                    first_index = i;
                } else if (current->value[i].begin_time.hour == ret.begin_time.hour) {
                    if (current->value[i].begin_time.minute < ret.begin_time.minute) {
                        ret = current->value[i];
                        first_index = i;
                    }
                }
            }
            return first_index;
        }
        current = current->next;
    }
    return first_index;
}

/**
 * @return the index of the next schedule item of the day, or -1 if there are none
 */
int has_next_schedule_item(DAY *day, CLOCK_TIME current_time) {
    unsigned int key = get_key(*day);
    NODE *current = head;
    SCHEDULE_ITEM earliest = {0};
    earliest.begin_time.hour = 25; // later than any possible hour
    int ret = -1;
    while (current != NULL) {

        // dont look here :d
        
        if (current->key == key) {           
            for (int i = 0; i < current->schedule_item_count; ++i) {
                if (current->value[i].begin_time.hour > current_time.hour) {
                    if (current->value[i].begin_time.hour < earliest.begin_time.hour) {
                        earliest = current->value[i];
                        ret = i;
                    } else if (current->value[i].begin_time.hour == earliest.begin_time.hour) {
                        if (current->value[i].begin_time.minute < earliest.begin_time.minute) {
                            earliest = current->value[i];
                            ret = i;
                        }
                    }
                } else if (current->value[i].begin_time.hour == current_time.hour) {
                    if (current->value[i].begin_time.minute > current_time.minute) {
                        if (current->value[i].begin_time.hour < earliest.begin_time.hour) {
                            earliest = current->value[i];
                            ret = i;
                        } else if (current->value[i].begin_time.hour == earliest.begin_time.hour) {
                            if (current->value[i].begin_time.minute < earliest.begin_time.minute) {
                                earliest = current->value[i];
                                ret = i;
                            }
                        }
                    }
                }
            }
            return ret;
        }
        current = current->next;
    }
    return ret;
}

SCHEDULE_ITEM* get_schedule_item(DAY *day, int index) {
    unsigned int key = get_key(*day);
    NODE *current = head;
    while (current != NULL) {
        if (current->key == key) {
            return &current->value[index];
        }
        current = current->next;
    }
    return NULL;
}