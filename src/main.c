#include <gtk/gtk.h>
#include "../header/startingPage.h"
#include "../header/gamePage.h"
#include "../header/gameConfig.h"

static GtkWidget *window;
static GtkWidget *startingPage;

static void click_quit_game_button(GtkWidget *gamePage);
static void click_help_button(GtkWidget *widget, gpointer data);
static void click_play_button(GtkWidget *widget, gpointer data);

static void apply_css(void)
{
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "src/styles.css");
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
}

static void click_play_button(GtkWidget *widget, gpointer data)
{
    gtk_window_set_child(GTK_WINDOW(window), NULL);
    GtkWidget *gamePage = create_game_page(window, click_quit_game_button);
    gtk_window_set_child(GTK_WINDOW(window), gamePage);
}

static void click_quit_game_button(GtkWidget *gamePage)
{
    startingPage = create_starting_page(click_play_button, click_help_button);
    GtkWidget *window = GTK_WIDGET(gtk_widget_get_root(gamePage));
    gtk_window_set_child(GTK_WINDOW(window), startingPage);
    // Ist hier noch clean up nötig für den GameContext.
    // Muss in den GameContext überhaupt die Mainpage oder passt das auch mit der GameArea?
}

static void click_help_button(GtkWidget *widget, gpointer data)
{
    g_print("Help\n");
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
