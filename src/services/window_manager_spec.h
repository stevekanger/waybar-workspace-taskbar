#pragma once

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _WindowManagerData WindowManagerData;
typedef struct WindowManagerEvent WindowManagerEvent;

#define WWT_WINDOW_MANAGER_SPEC_TYPE (wwt_window_manager_spec_get_type())

G_DECLARE_FINAL_TYPE(
    WwtWindowManagerSpec,
    wwt_window_manager_spec,
    WWT,
    WINDOW_MANAGER_SPEC,
    GObject
);

typedef enum WindowManagerId {
    WM_ID_UNSUPPORTED,
    WM_ID_SWAY,
    WM_ID_NIRI,
    WM_ID_HYPRLAND
} WindowManagerId;

typedef int (*WindowManagerEventsConstructor)();
typedef void (*WindowManagerEventsDestructor)(int fd, FILE *socket_file);
typedef gboolean (*WindowManagerEventsReader)(
    FILE *socket_file,
    WindowManagerEvent *event
);
typedef WindowManagerData *(*WindowManagerDataFetcher)();
typedef gboolean (*WindowManagerClickHandler)(const char *id);
typedef gboolean (*WindowManagerEventsValidator)(WindowManagerEvent *event);

typedef struct {
    WindowManagerEventsConstructor events_constructor;
    WindowManagerEventsDestructor events_destructor;
    WindowManagerEventsReader events_reader;
    WindowManagerEventsValidator events_validator;
    WindowManagerDataFetcher data_fetcher;
} WindowManagerSpecFactory;

WindowManagerEventsConstructor wwt_window_manager_spec_get_events_constructor(
    WwtWindowManagerSpec *self
);
WindowManagerEventsDestructor wwt_window_manager_spec_get_events_destructor(
    WwtWindowManagerSpec *self
);
WindowManagerEventsReader wwt_window_manager_spec_get_events_reader(
    WwtWindowManagerSpec *self
);
WindowManagerEventsValidator wwt_window_manager_spec_get_events_validator(
    WwtWindowManagerSpec *self
);
WindowManagerDataFetcher wwt_window_manager_spec_get_data_fetcher(
    WwtWindowManagerSpec *self
);

WwtWindowManagerSpec *wwt_window_manager_spec_new(WindowManagerId wm_id);

G_END_DECLS
