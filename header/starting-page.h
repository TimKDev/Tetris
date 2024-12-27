#ifndef STARTING_PAGE_H
#define STARTING_PAGE_H

#include <gtk/gtk.h>

// Function to create and return the starting page box
GtkWidget* create_starting_page(void(*startCallback)(GtkWidget *widget, gpointer data), void(*helpCallback)(GtkWidget *widget, gpointer data));

#endif 