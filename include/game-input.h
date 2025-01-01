#pragma once

#include <gtk/gtk.h>
#include "render-logic.h"

gboolean handle_key_press(GtkEventControllerKey *controller, guint keyval,
                          guint keycode, GdkModifierType state, gpointer user_data);
