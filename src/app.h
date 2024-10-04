#ifndef APP_H
#define APP_H

typedef enum app_view {
    WEEK_VIEW,
    MONTH_VIEW
} APP_VIEW;

typedef struct app_state {
    APP_VIEW view_type;
    int use_24h_format; // I'll forllow in Tuhkis steps, and also use int instead of bool
} APP_STATE;

// Global data.
extern APP_STATE app;

#endif // APP_H

