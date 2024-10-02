#ifndef APP_H
#define APP_H

typedef enum app_view {
    WEEK_VIEW,
    MONTH_VIEW
} APP_VIEW;

typedef struct app_state {
    APP_VIEW view_type;
} APP_STATE;

// Global data.
extern APP_STATE app;

#endif // APP_H

