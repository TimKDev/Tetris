#include "render-logic.h"
#include "game-over-dialog.h"
#include "game-context.h"

static void on_game_over_response(GtkDialog *dialog, int response, gpointer user_data);

void show_game_over_dialog(GameContext *context)
{
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Game Over",
        GTK_WINDOW(gtk_widget_get_root(context->game_area)),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "Restart", GTK_RESPONSE_ACCEPT,
        "Quit", GTK_RESPONSE_REJECT,
        NULL);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    char score_text[100];
    snprintf(score_text, sizeof(score_text), "Game Over!\nYour score: %d", context->game_data->score);
    GtkWidget *label = gtk_label_new(score_text);
    gtk_widget_set_margin_start(label, 20);
    gtk_widget_set_margin_end(label, 20);
    gtk_widget_set_margin_top(label, 20);
    gtk_widget_set_margin_bottom(label, 20);

    gtk_box_append(GTK_BOX(content_area), label);

    g_signal_connect(dialog, "response", G_CALLBACK(on_game_over_response), context);
    gtk_widget_show(dialog);
}

static void on_game_over_response(GtkDialog *dialog, int response, gpointer user_data)
{
    GameContext *context = (GameContext *)user_data;

    if (response == GTK_RESPONSE_ACCEPT)
    {
        restart_game(context);
    }
    else
    {
        quit_game(context);
    }

    gtk_window_destroy(GTK_WINDOW(dialog));
}