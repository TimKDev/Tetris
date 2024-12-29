#ifndef HELP_PAGE_H
#define HELP_PAGE_H

#include <gtk/gtk.h>

GtkWidget *create_help_page(void (*quitCallback)(GtkWidget *widget));

#endif