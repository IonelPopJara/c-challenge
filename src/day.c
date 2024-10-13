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

/**
 * Get the current local time
 * @return The current local time
 */
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
 * 
 * ==============================================
 * The behemoth was witnessed and 50 lines were cut off :D
 * 
 * It's still a mess, but at least it's a smaller mess now 
 * 
 * remove_schedule_item() used in the delete button for each item (main.c)
 * 
 * /Flameo(Flam30)
 */

typedef struct node {
    unsigned int key; // key for the day, example 01-02-2021 is 20210201
    SCHEDULE_ITEM value[DAY_MAX_SCHEDULE_ITEMS];
    int schedule_item_count;
    struct node *next;
} NODE;

NODE *head = NULL;

// Helper: Get the key for a given day
unsigned int get_key(DAY day) {
    return day.date.day + day.date.month * 100 + day.date.year * 10000;
}

// Helper: Find the node for a given day key
NODE* find_node(unsigned int key) {
    NODE *current = head;
    while (current != NULL) {
        if (current->key == key) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Helper: Compare two CLOCK_TIME values
int compare_time(CLOCK_TIME t1, CLOCK_TIME t2) {
    if (t1.hour != t2.hour) {
        return t1.hour - t2.hour;
    }
    return t1.minute - t2.minute;
}

// Check if the day exists in the list
int is_day_in_list(DAY *day) {
    return find_node(get_key(*day)) != NULL;
}

/**
 * Add a schedule item to the day
 * @param day The day to add the schedule item to
 * @param item The schedule item to add
*/ 
void add_schedule_item(DAY *day, SCHEDULE_ITEM item) {
    unsigned int key = get_key(*day);
    NODE *node = find_node(key);

    if (node != NULL) {
        if (node->schedule_item_count < DAY_MAX_SCHEDULE_ITEMS) {
            node->value[node->schedule_item_count++] = item;
        }
    } else {
        NODE *new_node = malloc(sizeof(NODE));
        new_node->key = key;
        new_node->value[0] = item;
        new_node->schedule_item_count = 1;
        new_node->next = head;
        head = new_node;
    }
}

/**
 * Remove a schedule item from the day
 * @param day The day to remove the schedule item from
 * @param index The index of the schedule item to remove
*/ 
void remove_schedule_item(DAY *day, int index) {
    unsigned int key = get_key(*day);
    NODE *node = find_node(key);
    if (node != NULL && index >= 0 && index < node->schedule_item_count) {
        for (int i = index; i < node->schedule_item_count - 1; ++i) {
            node->value[i] = node->value[i + 1];
        }
        node->schedule_item_count--;
    }
}

/**
 * Check if a day has no schedule items
 * @return 1 if the day is empty, 0 otherwise
*/
int is_day_empty(DAY *day) {
    NODE *node = find_node(get_key(*day));
    return (node == NULL || node->schedule_item_count == 0);
}

/** 
 * Find the index of the first schedule item in a day
 * @return -1 if there are no schedule items in the day
*/
int first_schedule_item(DAY *day) {
    NODE *node = find_node(get_key(*day));
    if (node == NULL || node->schedule_item_count == 0) return -1;

    int first_index = 0;
    for (int i = 1; i < node->schedule_item_count; ++i) {
        if (compare_time(node->value[i].begin_time, node->value[first_index].begin_time) < 0) {
            first_index = i;
        }
    }
    return first_index;
}

/**
 * Find the index of the next schedule item in a day, after a given time
 * @return -1 if there are no more schedule items
 *  
*/ 
int has_next_schedule_item(DAY *day, CLOCK_TIME current_time) {
    NODE *node = find_node(get_key(*day));
    if (node == NULL || node->schedule_item_count == 0) return -1;

    int next_index = -1;
    for (int i = 0; i < node->schedule_item_count; ++i) {
        if (compare_time(node->value[i].begin_time, current_time) > 0) {
            if (next_index == -1 || compare_time(node->value[i].begin_time, node->value[next_index].begin_time) < 0) {
                next_index = i;
            }
        }
    }
    return next_index;
}

/**
 * Get a specific schedule item by index
 * @return NULL if the index is out of bounds
*/ 
SCHEDULE_ITEM* get_schedule_item(DAY *day, int index) {
    NODE *node = find_node(get_key(*day));
    if (node != NULL && index >= 0 && index < node->schedule_item_count) {
        return &node->value[index];
    }
    return NULL;
}

/*
 * Get the number of schedule items in a day
 * @return int - the number of schedule items 
*/ 
int get_schedule_item_count(DAY *day) {
    NODE *node = find_node(get_key(*day));
    return (node == NULL) ? 0 : node->schedule_item_count;
}