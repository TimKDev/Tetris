#include <gtk/gtk.h>
#include "game-input.h"
#include "game-context.h"
#include "game-logic.h"
#include "render-logic.h"
#include "game-loop.h"
#include "game-ui.h"

#define BORDER_WIDTH 10
#define COLS_NEXT_PIECE 5
#define ROWS_NEXT_PIECE 5

static void on_pause_button_clicked(GtkButton *button, gpointer user_data);
static void on_restart_button_clicked(GtkButton *button, gpointer user_data);
static void on_quit_button_clicked(GtkButton *button, gpointer user_data);
static GtkWidget *create_game_page_widget();
static GtkWidget *create_game_area_widget();
static GtkWidget *create_side_panel_with_buttons(GameContext *game_context);

GtkWidget *create_game_page(GtkWidget *window, char *playerName, void (*quitCallback)(GtkWidget *widget))
{
    GtkWidget *game_page = create_game_page_widget();
    GtkWidget *game_area = create_game_area_widget();
    GameContext *game_context = create_game_context(game_page, game_area, playerName, quitCallback);
    GtkWidget *side_panel = create_side_panel_with_buttons(game_context);

    GtkEventController *key_controller = gtk_event_controller_key_new();
    game_context->key_controller = key_controller;
    game_context->key_controller_handler_id = g_signal_connect(
        key_controller,
        "key-pressed",
        G_CALLBACK(handle_key_press),
        game_context);
    gtk_widget_add_controller(window, key_controller);

    game_context->update_game_handler_id = g_timeout_add(
        10,
        G_SOURCE_FUNC(update_game),
        game_context);

    gtk_box_append(GTK_BOX(game_page), game_area);
    gtk_box_append(GTK_BOX(game_page), side_panel);

    return game_page;
}

void destroy_game_page_ui_elements(GameContext *gameContext)
{

    if (gameContext->update_game_handler_id > 0)
    {
        g_source_remove(gameContext->update_game_handler_id);
        gameContext->update_game_handler_id = 0;
    }

    if (gameContext->key_controller_handler_id > 0 && gameContext->key_controller)
    {
        g_signal_handler_disconnect(
            gameContext->key_controller,
            gameContext->key_controller_handler_id);
        gameContext->key_controller_handler_id = 0;
    }

    if (gameContext->key_controller && gameContext->window)
    {
        gtk_widget_remove_controller(
            gameContext->window,
            gameContext->key_controller);
    }
}

static void on_pause_button_clicked(GtkButton *button, gpointer user_data)
{
    GameContext *context = (GameContext *)user_data;
    context->is_paused = !context->is_paused;
    const char *new_label = context->is_paused ? "Resume" : "Pause";
    gtk_button_set_label(button, new_label);
}

static void on_restart_button_clicked(GtkButton *button, gpointer user_data)
{
    GameContext *context = (GameContext *)user_data;
    restart_game(context);
}

static void on_quit_button_clicked(GtkButton *button, gpointer user_data)
{
    GameContext *context = (GameContext *)user_data;
    quit_game(context);
}

static GtkWidget *create_game_page_widget()
{
    GtkWidget *game_page = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    gtk_widget_set_margin_start(game_page, 20);
    gtk_widget_set_margin_end(game_page, 20);
    gtk_widget_set_margin_top(game_page, 20);
    gtk_widget_set_margin_bottom(game_page, 20);

    return game_page;
}

static GtkWidget *create_game_area_widget()
{
    GtkWidget *game_area = gtk_fixed_new();
    gtk_widget_set_size_request(game_area, GAME_WIDTH + 2 * BORDER_WIDTH, GAME_HEIGHT + 2 * BORDER_WIDTH);
    gtk_widget_add_css_class(game_area, "game-area");

    // Make the widget non-expandable
    gtk_widget_set_halign(game_area, GTK_ALIGN_START);
    gtk_widget_set_valign(game_area, GTK_ALIGN_START);
    gtk_widget_set_hexpand(game_area, FALSE);
    gtk_widget_set_vexpand(game_area, FALSE);

    return game_area;
}

static GtkWidget *create_side_panel_with_buttons(GameContext *game_context)
{
    GtkWidget *side_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_start(side_panel, 20);

    // Create and store score label in context
    game_context->score_label = gtk_label_new("Score: 0");
    gtk_widget_add_css_class(game_context->score_label, "score-label");
    gtk_widget_set_halign(game_context->score_label, GTK_ALIGN_START);

    char *playerNameLabel = g_strdup_printf("Player Name: %s", game_context->game_data->playerName);
    GtkWidget *playerName = gtk_label_new(playerNameLabel);
    gtk_widget_add_css_class(playerName, "score-label");
    gtk_widget_set_halign(playerName, GTK_ALIGN_START);

    // Add next piece preview
    GtkWidget *next_piece_label = gtk_label_new("Next Piece:");
    gtk_widget_set_halign(next_piece_label, GTK_ALIGN_START);
    GtkWidget *next_piece_area = gtk_fixed_new();
    gtk_widget_set_size_request(next_piece_area, ROWS_NEXT_PIECE * BLOCK_SIZE, COLS_NEXT_PIECE * BLOCK_SIZE);
    gtk_widget_add_css_class(next_piece_area, "game-area");

    gtk_widget_set_halign(next_piece_area, GTK_ALIGN_START);
    gtk_widget_set_valign(next_piece_area, GTK_ALIGN_START);
    gtk_widget_set_hexpand(next_piece_area, FALSE);
    gtk_widget_set_vexpand(next_piece_area, FALSE);

    game_context->next_piece_area = next_piece_area;

    // Add elements to side panel
    gtk_box_append(GTK_BOX(side_panel), game_context->score_label);
    gtk_box_append(GTK_BOX(side_panel), playerName);
    gtk_box_append(GTK_BOX(side_panel), next_piece_label);
    gtk_box_append(GTK_BOX(side_panel), next_piece_area);

    // Create pause button
    GtkWidget *pause_button = gtk_button_new_with_label("Pause");
    gtk_widget_set_halign(pause_button, GTK_ALIGN_START);
    g_signal_connect(pause_button, "clicked", G_CALLBACK(on_pause_button_clicked), game_context);

    // Create restart button
    GtkWidget *restart_button = gtk_button_new_with_label("Restart");
    gtk_widget_set_halign(restart_button, GTK_ALIGN_START);
    g_signal_connect(restart_button, "clicked", G_CALLBACK(on_restart_button_clicked), game_context);

    // Create quit button
    GtkWidget *quit_button = gtk_button_new_with_label("Quit");
    g_signal_connect(
        quit_button,
        "clicked",
        G_CALLBACK(on_quit_button_clicked),
        game_context);

    // Add elements to side panel (add this after other side panel elements)
    gtk_box_append(GTK_BOX(side_panel), pause_button);
    gtk_box_append(GTK_BOX(side_panel), restart_button);
    gtk_box_append(GTK_BOX(side_panel), quit_button);

    return side_panel;
}

void update_score_display(GameContext *context)
{
    if (!context || !context->score_label)
        return;

    char *score_text = g_strdup_printf("Score: %d", context->game_data->score);
    gtk_label_set_text(GTK_LABEL(context->score_label), score_text);
    g_free(score_text);
}