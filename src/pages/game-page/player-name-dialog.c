#include <gtk/gtk.h>

void show_name_dialog(GtkWidget *parent_window, void(on_name_dialog_response)(GtkDialog *dialog, gint response_id, gpointer user_data))
{
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *name_entry;
    GtkWidget *label;
    GtkWidget *box;

    // Create dialog
    dialog = gtk_dialog_new_with_buttons(
        "Enter Player Name",
        GTK_WINDOW(parent_window),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "Start Game",
        GTK_RESPONSE_ACCEPT,
        "Cancel",
        GTK_RESPONSE_CANCEL,
        NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Create a vertical box for the content
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_start(box, 20);
    gtk_widget_set_margin_end(box, 20);
    gtk_widget_set_margin_top(box, 20);
    gtk_widget_set_margin_bottom(box, 20);

    // Add label
    label = gtk_label_new("Please enter your name:");
    gtk_box_append(GTK_BOX(box), label);

    // Add text entry
    name_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(name_entry), 50);
    gtk_entry_set_placeholder_text(GTK_ENTRY(name_entry), "Player name");
    gtk_box_append(GTK_BOX(box), name_entry);

    gtk_box_append(GTK_BOX(content_area), box);
    gtk_widget_show(dialog);

    g_signal_connect(dialog, "response", G_CALLBACK(on_name_dialog_response), name_entry);
}