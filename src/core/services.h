#pragma once

#include "core/config.h"
#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _WwtWindowManagerSpec WwtWindowManagerSpec;
typedef struct _WwtWindowManagerEvents WwtWindowManagerEvents;
typedef struct _WwtAppIcons WwtAppIcons;

#define WWT_SERVICES_TYPE (wwt_services_get_type())

G_DECLARE_FINAL_TYPE(WwtServices, wwt_services, WWT, SERVICES, GObject)

WwtServices *wwt_services_init_default(WindowManagerId wm_id);
WwtServices *wwt_services_get_default();
WwtWindowManagerSpec *wwt_services_get_window_manager_spec(WwtServices *self);
WwtAppIcons *wwt_services_get_app_icons(WwtServices *self);
WwtWindowManagerEvents *wwt_services_get_window_manager_events(
    WwtServices *self
);

G_END_DECLS
