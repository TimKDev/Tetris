#pragma once

#include <gtk/gtk.h>

void show_name_dialog(GtkWidget *parent_window, void(on_name_dialog_response)(GtkDialog *dialog, gint response_id, gpointer user_data));
