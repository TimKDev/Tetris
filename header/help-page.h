#ifndef HELP_PAGE_H
#define HELP_PAGE_H

#include <gtk/gtk.h>

// Function to create and return the starting page box
GtkWidget *create_help_page(void (*quitCallback)(GtkWidget *widget, gpointer data));

#endif