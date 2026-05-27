#pragma once

#include "window_manager_spec.h"
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define WM_EVENTS_MAX_CALlBACKS 8

typedef struct _WindowManagerData WindowManagerData;
typedef struct _WwtWindowManagerEvents WwtWindowManagerEvents;

#define WWT_WINDOW_MANAGER_EVENTS_TYPE (wwt_window_manager_events_get_type())

G_DECLARE_FINAL_TYPE(
    WwtWindowManagerEvents,
    wwt_window_manager_events,
    WWT,
    WINDOW_MANAGER_EVENTS,
    GObject
);

typedef struct WindowManagerEvent {
    char *msg;
    size_t msg_len;
    size_t msg_size;
    gint debounce_timeout_id;
} WindowManagerEvent;

typedef void (*WindowManagerEventsCallback)(
    WindowManagerEvent *event,
    gpointer user_data
);

typedef struct WindowManagerEventsSubscription {
    WindowManagerEventsCallback cb;
    gpointer user_data;
} WindowManagerEventsSubscription;

WwtWindowManagerEvents *wwt_window_manager_events_new(
    WindowManagerEventsConstructor events_constructor,
    WindowManagerEventsDestructor events_destructor,
    WindowManagerEventsReader events_reader,
    WindowManagerEventsValidator events_validator
);

int window_manager_events_subscribe(
    WwtWindowManagerEvents *events,
    WindowManagerEventsCallback cb,
    gpointer user_data
);

int window_manager_events_unsubscribe(WwtWindowManagerEvents *events, int pos);

G_END_DECLS
