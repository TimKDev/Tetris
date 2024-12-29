#include <gtk/gtk.h>
#include "starting-page.h"
#include "game-page.h"
#include "player-name-dialog.h"
#include "help-page.h"

static GtkWidget *window;
static GtkWidget *startingPage;

static void click_quit_game_button(GtkWidget *gamePage);
static void click_help_button(GtkWidget *widget, gpointer data);
static void click_play_button(GtkWidget *widget, gpointer data);
static void on_name_dialog_response(GtkDialog *dialog, gint response_id, gpointer user_data);

static void apply_css(void)
{
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "src/styles.css");
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
}

static void on_name_dialog_response(GtkDialog *dialog, gint response_id, gpointer user_data)
{
    GtkEntry *name_entry = GTK_ENTRY(user_data);
    GtkEntryBuffer *name_buffer = gtk_entry_get_buffer(name_entry);

    if (response_id == GTK_RESPONSE_ACCEPT)
    {
        const char *name = gtk_entry_buffer_get_text(name_buffer);
        if (strlen(name) > 0)
        {
            char *player_name = g_strdup(name);
            gtk_window_set_child(GTK_WINDOW(window), NULL);
            GtkWidget *gamePage = create_game_page(window, player_name, click_quit_game_button);
            gtk_window_set_child(GTK_WINDOW(window), gamePage);
        }
    }

    gtk_window_destroy(GTK_WINDOW(dialog));
}

static void click_play_button(GtkWidget *widget, gpointer data)
{
    GtkWidget *parent_window = GTK_WIDGET(gtk_widget_get_root(widget));
    show_name_dialog(parent_window, on_name_dialog_response);
}

static void click_quit_game_button(GtkWidget *gamePage)
{
    startingPage = create_starting_page(click_play_button, click_help_button);
    GtkWidget *window = GTK_WIDGET(gtk_widget_get_root(gamePage));
    gtk_window_set_child(GTK_WINDOW(window), startingPage);
}

static void click_help_button(GtkWidget *widget, gpointer data)
{
    gtk_window_set_child(GTK_WINDOW(window), NULL);
    GtkWidget *helpPage = create_help_page(click_quit_game_button);
    gtk_window_set_child(GTK_WINDOW(window), helpPage);
}

static void activate(GtkApplication *app, gpointer user_data)
{
    apply_css();
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Tetris");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 500);

    startingPage = create_starting_page(click_play_button, click_help_button);

    gtk_window_set_child(GTK_WINDOW(window), startingPage);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv)
{
    GtkApplication *app = gtk_application_new("org.example.hello", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
