#include "services.h"
#include "config.h"
#include "services/app_icons.h"
#include "services/window_manager_events.h"
#include "services/window_manager_spec.h"
#include <assert.h>

typedef enum InitStatus {
    INIT_STATUS_PENDING,
    INIT_STATUS_FAILED,
    INIT_STATUS_SUCCESS
} InitStatus;

struct _WwtServices {
    GObject parent_instance;

    WwtWindowManagerSpec *window_manager_spec;
    WwtWindowManagerEvents *window_manager_events;
    WwtAppIcons *app_icons;
};

G_DEFINE_TYPE(WwtServices, wwt_services, G_TYPE_OBJECT);

static WwtServices *services_instance = NULL;
static InitStatus init_status = INIT_STATUS_PENDING;

/**
 * Gets the app icons
 *
 * @param self
 * @return The app icons
 */
WwtAppIcons *wwt_services_get_app_icons(WwtServices *self) {
    return self->app_icons;
}

/**
 * Gets the window manager events
 *
 * @param self
 * @return The window events
 */
WwtWindowManagerEvents *wwt_services_get_window_manager_events(
    WwtServices *self
) {
    return self->window_manager_events;
}

/**
 * Gets the window manager events
 *
 * @param self
 * @return The window events
 */
WwtWindowManagerSpec *wwt_services_get_window_manager_spec(WwtServices *self) {
    return self->window_manager_spec;
}

/**
 * Handles object disposal
 *
 * @param obj The object struct
 */
static void dispose(GObject *obj) {
    WwtServices *self = WWT_SERVICES(obj);

    g_clear_object(&self->window_manager_events);
    g_clear_object(&self->window_manager_spec);
    g_clear_object(&self->app_icons);

    if(services_instance) {
        services_instance = NULL;
        init_status = INIT_STATUS_PENDING;
    }

    G_OBJECT_CLASS(wwt_services_parent_class)->dispose(obj);
}

/**
 * Handles object finalization
 *
 * @param obj The obj struct
 */
static void finalize(GObject *obj) {
    G_OBJECT_CLASS(wwt_services_parent_class)->finalize(obj);
}

/**
 * Initialize the object
 *
 * @param self
 */
static void wwt_services_init(WwtServices *self) {
}

/**
 * Initialized the class instance and methods
 *
 * @param class the class instance
 */
static void wwt_services_class_init(WwtServicesClass *klass) {
    G_OBJECT_CLASS(klass)->dispose = dispose;
    G_OBJECT_CLASS(klass)->finalize = finalize;
}

/**
 * Gets the already created instance
 *
 * @return The services instance
 */
WwtServices *wwt_services_get_default() {
    assert(services_instance != NULL);

    return services_instance;
}

/**
 * Creates or get the services instance
 *
 * @param config The WwtConfig instance
 * @return (transfer: full) The services instance (caller gets a ref)
 */
WwtServices *wwt_services_init_default(WindowManagerId wm_id) {
    if(init_status == INIT_STATUS_FAILED) {
        return NULL;
    }

    if(services_instance && init_status == INIT_STATUS_SUCCESS) {
        g_object_ref(services_instance);
        return services_instance;
    }

    WwtServices *self = g_object_new(WWT_SERVICES_TYPE, NULL);

    // Initialize the spec first
    self->window_manager_spec = wwt_window_manager_spec_new(wm_id);
    if(!self->window_manager_spec) {
        init_status = INIT_STATUS_FAILED;
        g_object_unref(self);
        return NULL;
    }

    self->window_manager_events = wwt_window_manager_events_new(
        wwt_window_manager_spec_get_events_constructor(
            self->window_manager_spec
        ),
        wwt_window_manager_spec_get_events_destructor(
            self->window_manager_spec
        ),
        wwt_window_manager_spec_get_events_reader(self->window_manager_spec),
        wwt_window_manager_spec_get_events_validator(self->window_manager_spec)
    );
    if(!self->window_manager_events) {
        init_status = INIT_STATUS_FAILED;
        g_object_unref(self);
        return NULL;
    }

    self->app_icons = wwt_app_icons_new();
    if(!self->app_icons) {
        init_status = INIT_STATUS_FAILED;
        g_object_unref(self);
        return NULL;
    }

    services_instance = self;
    init_status = INIT_STATUS_SUCCESS;

    return self;
}
