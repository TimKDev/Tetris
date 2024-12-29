#include "help-page.h"

GtkWidget *create_help_page(void (*quitCallback)(GtkWidget *widget))
{
    GtkWidget *helpPage = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_widget_set_valign(helpPage, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_start(helpPage, 20);
    gtk_widget_set_margin_end(helpPage, 20);
    gtk_widget_set_margin_top(helpPage, 20);
    gtk_widget_set_margin_bottom(helpPage, 20);

    GtkWidget *title = gtk_label_new("Help");
    gtk_widget_add_css_class(title, "title");

    // Controls explanation
    const char *controls_text =
        "← / →  :  Move piece left/right\n"
        "↓      :  Accelerate piece descent\n"
        "↑      :  Rotate piece\n";

    GtkWidget *text = gtk_label_new(controls_text);

    // Quit button
    GtkWidget *quitButton = gtk_button_new_with_label("Back to Menu");
    gtk_widget_set_halign(quitButton, GTK_ALIGN_CENTER);

    g_signal_connect(quitButton, "clicked", G_CALLBACK(quitCallback), NULL);

    gtk_box_append(GTK_BOX(helpPage), title);
    gtk_box_append(GTK_BOX(helpPage), text);
    gtk_box_append(GTK_BOX(helpPage), quitButton);

    return helpPage;
}