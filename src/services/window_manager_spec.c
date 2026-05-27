#include "window_manager_spec.h"
#include "window_managers/hyprland.h"
#include "window_managers/niri.h"
#include "window_managers/sway.h"

struct _WwtWindowManagerSpec {
    GObject parent_instance;

    WindowManagerId id;
    WindowManagerEventsConstructor events_constructor;
    WindowManagerEventsDestructor events_destructor;
    WindowManagerEventsReader events_reader;
    WindowManagerEventsValidator events_validator;
    WindowManagerDataFetcher data_fetcher;
};

G_DEFINE_TYPE(WwtWindowManagerSpec, wwt_window_manager_spec, G_TYPE_OBJECT);

/**
 * Gets the window manager id
 *
 * @param self
 * @return The id
 */
WindowManagerId wwt_window_manager_spec_get_id(WwtWindowManagerSpec *self) {
    return self->id;
}

/**
 * Gets the events_constructor
 *
 * @param self
 * @return The events_constructor
 */
WindowManagerEventsConstructor wwt_window_manager_spec_get_events_constructor(
    WwtWindowManagerSpec *self
) {
    return self->events_constructor;
}

/**
 * Gets the events_destructor
 *
 * @param self
 * @return The events_destructor
 */
WindowManagerEventsDestructor wwt_window_manager_spec_get_events_destructor(
    WwtWindowManagerSpec *self
) {
    return self->events_destructor;
}

/**
 * Gets the events_reader
 *
 * @param self
 * @return The events_reader
 */
WindowManagerEventsReader wwt_window_manager_spec_get_events_reader(
    WwtWindowManagerSpec *self
) {
    return self->events_reader;
}

/**
 * Gets the events_validator
 *
 * @param self
 * @return The events_validator
 */
WindowManagerEventsValidator wwt_window_manager_spec_get_events_validator(
    WwtWindowManagerSpec *self
) {
    return self->events_validator;
}

/**
 * Gets the data_fetcher
 *
 * @param self
 * @return The data_fetcher
 */
WindowManagerDataFetcher wwt_window_manager_spec_get_data_fetcher(
    WwtWindowManagerSpec *self
) {
    return self->data_fetcher;
}

/**
 * Initialize the instance
 *
 * @param self
 */
static void wwt_window_manager_spec_init(WwtWindowManagerSpec *self) {
}

/**
 * Class initializer
 *
 * @param klass the object class
 */
static void wwt_window_manager_spec_class_init(
    WwtWindowManagerSpecClass *klass
) {
}

/**
 * Creates the window manager spec
 *
 * @param wm_id The window manager id
 * @return (transfer full) The window manager spec instance
 */
WwtWindowManagerSpec *wwt_window_manager_spec_new(WindowManagerId id) {
    WwtWindowManagerSpec *self =
        g_object_new(WWT_WINDOW_MANAGER_SPEC_TYPE, NULL);
    self->id = id;

    WindowManagerSpecFactory factory;
    if(id == WM_ID_NIRI) {
        factory = window_manager_spec_factory_niri();
    } else if(id == WM_ID_HYPRLAND) {
        factory = window_manager_spec_factory_hyprland();
    } else if(id == WM_ID_SWAY) {
        factory = window_manager_spec_factory_sway();
    } else {
        g_object_unref(self);
        return NULL;
    }

    self->events_validator = factory.events_validator;
    self->events_reader = factory.events_reader;
    self->events_constructor = factory.events_constructor;
    self->events_destructor = factory.events_destructor;
    self->data_fetcher = factory.data_fetcher;

    return self;
}
