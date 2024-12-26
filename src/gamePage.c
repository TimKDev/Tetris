#include "gamePage.h"
#include "gameLogic.h"
#include "renderLogic.h"
#include "gameConfig.h"
#include "blockMovement.h"

#define BORDER_WIDTH 10

static gboolean update_game(GameContext *data);
void setup_key_controls(GtkWidget *window, GameContext *context);
static gboolean on_key_pressed(GtkEventControllerKey *controller,
                               guint keyval,
                               guint keycode,
                               GdkModifierType state,
                               gpointer user_data);
static void on_pause_button_clicked(GtkButton *button, gpointer user_data);
GameContext *create_game_context(GtkWidget *gamePage, GtkWidget *gameArea, void (*quitCallback)(GtkWidget *widget));
void destroy_game_context(GameContext *gameContext);
void restart_game(GameContext *context);
static void on_restart_button_clicked(GtkButton *button, gpointer user_data);
static void on_game_over_response(GtkDialog *dialog, int response, gpointer user_data);
static void on_quit_button_clicked(GtkButton *button, gpointer user_data);
static void show_game_over_dialog(GameContext *context);
void quit_game(GameContext *gameContext);

GtkWidget *create_game_page(GtkWidget *window, void (*quitCallback)(GtkWidget *widget))
{
    GtkWidget *game_page;
    GtkWidget *game_area;
    GtkWidget *side_panel;
    GtkWidget *score_label;
    GtkWidget *next_piece_label;
    static int counter = 0;

    // Create horizontal box container for main layout
    game_page = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    gtk_widget_set_margin_start(game_page, 20);
    gtk_widget_set_margin_end(game_page, 20);
    gtk_widget_set_margin_top(game_page, 20);
    gtk_widget_set_margin_bottom(game_page, 20);

    // Create game area (left side)
    game_area = gtk_fixed_new();
    gtk_widget_set_size_request(game_area, GAME_WIDTH + 2 * BORDER_WIDTH, GAME_HEIGHT + 2 * BORDER_WIDTH);
    gtk_widget_add_css_class(game_area, "game-area");

    // Make the widget non-expandable
    gtk_widget_set_halign(game_area, GTK_ALIGN_START);
    gtk_widget_set_valign(game_area, GTK_ALIGN_START);
    gtk_widget_set_hexpand(game_area, FALSE);
    gtk_widget_set_vexpand(game_area, FALSE);

    GameContext *game_context = create_game_context(game_page, game_area, quitCallback);

    GtkEventController *key_controller = gtk_event_controller_key_new();
    g_signal_connect(key_controller, "key-pressed", G_CALLBACK(on_key_pressed), game_context);
    gtk_widget_add_controller(window, key_controller);

    // Create a timer function that will be called every second
    g_timeout_add(10, G_SOURCE_FUNC(update_game), game_context);

    // Create side panel (right side)
    side_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_start(side_panel, 20);

    // Add score display
    score_label = gtk_label_new("Score: 0");
    gtk_widget_add_css_class(score_label, "score-label");
    gtk_widget_set_halign(score_label, GTK_ALIGN_START);

    // Add next piece preview
    next_piece_label = gtk_label_new("Next Piece:");
    gtk_widget_set_halign(next_piece_label, GTK_ALIGN_START);
    GtkWidget *next_piece_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(next_piece_area, 100, 100);
    gtk_widget_add_css_class(next_piece_area, "next-piece");

    // Add elements to side panel
    gtk_box_append(GTK_BOX(side_panel), score_label);
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
    gtk_widget_set_halign(quit_button, GTK_ALIGN_START);
    g_signal_connect(quit_button, "clicked", G_CALLBACK(on_quit_button_clicked), game_context);

    // Add elements to side panel (add this after other side panel elements)
    gtk_box_append(GTK_BOX(side_panel), pause_button);
    gtk_box_append(GTK_BOX(side_panel), restart_button);
    gtk_box_append(GTK_BOX(side_panel), quit_button);

    // Add main elements to box
    gtk_box_append(GTK_BOX(game_page), game_area);
    gtk_box_append(GTK_BOX(game_page), side_panel);

    return game_page;
}

static gboolean update_game(GameContext *gameContext)
{
    if (!gameContext->is_paused)
    {
        nextMove(gameContext->game_data, gameContext->config);
        render_game_data(gameContext);
        if (gameContext->game_data->gameOver)
        {
            gameContext->is_paused = true;
            show_game_over_dialog(gameContext);
        }
    }
    return G_SOURCE_CONTINUE;
}

// Callback function for key events
static gboolean on_key_pressed(GtkEventControllerKey *controller,
                               guint keyval,
                               guint keycode,
                               GdkModifierType state,
                               gpointer user_data)
{
    GameContext *context = (GameContext *)user_data;

    if (context->is_paused)
    {
        return FALSE;
    }

    switch (keyval)
    {
    case GDK_KEY_Left:
        move_piece_left(context->game_data);
        break;

    case GDK_KEY_Right:
        move_piece_right(context->game_data);
        break;

    case GDK_KEY_Up:
        printf("Up arrow pressed\n");
        break;

    case GDK_KEY_Down:
        move_piece_down(context->game_data);
        break;

    default:
        return FALSE; // Event not handled
    }

    render_game_data(context);
    return TRUE; // Event handled
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

void restart_game(GameContext *context)
{
    GtkWidget *gameArea = context->game_area;

    if (context->render_state)
    {
        destroy_render_state(context->render_state, gameArea);
        context->render_state = NULL;
    }

    if (context->game_data)
    {
        destory_game_data(context->game_data);
        context->game_data = NULL;
    }

    if (context->config)
    {
        destroy_game_config(context->config);
        context->config = NULL;
    }

    context->config = load_config_from_file("config/game.json");
    context->game_data = initialize(context->config);
    context->render_state = create_render_state();
    context->is_paused = false;

    render_game_data(context);
}

GameContext *create_game_context(GtkWidget *gamePage, GtkWidget *gameArea, void (*quitCallback)(GtkWidget *widget))
{
    GameConfig *config = load_config_from_file("config/game.json");
    GameData *game_data = initialize(config);
    RenderState *render_state = create_render_state();
    GameContext *game_context = (GameContext *)malloc(sizeof(GameContext));
    game_context->game_area = gameArea;
    game_context->game_data = game_data;
    game_context->render_state = render_state;
    game_context->config = config;
    game_context->is_paused = false;
    game_context->quit_callback = quitCallback;

    return game_context;
}

void destroy_game_context(GameContext *gameContext)
{
    destroy_game_config(gameContext->config);
    destory_game_data(gameContext->game_data);
    destroy_render_state(gameContext->render_state, gameContext->game_area);
    free(gameContext);
    gameContext = NULL;
}

static void show_game_over_dialog(GameContext *context)
{
    // Create dialog
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Game Over",
        GTK_WINDOW(gtk_widget_get_root(context->game_area)),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "Restart", GTK_RESPONSE_ACCEPT,
        "Quit", GTK_RESPONSE_REJECT,
        NULL);

    // Create content area
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Create score message
    char score_text[100];
    snprintf(score_text, sizeof(score_text), "Game Over!\nYour score: %d", context->game_data->score);
    GtkWidget *label = gtk_label_new(score_text);
    gtk_widget_set_margin_start(label, 20);
    gtk_widget_set_margin_end(label, 20);
    gtk_widget_set_margin_top(label, 20);
    gtk_widget_set_margin_bottom(label, 20);

    // Add label to dialog
    gtk_box_append(GTK_BOX(content_area), label);

    // Show dialog and handle response
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
        GtkWidget *window = GTK_WIDGET(gtk_widget_get_root(context->game_area));
        gtk_window_destroy(GTK_WINDOW(window));
    }

    gtk_window_destroy(GTK_WINDOW(dialog));
}

void quit_game(GameContext *gameContext)
{
    destroy_game_config(gameContext->config);
    destory_game_data(gameContext->game_data);
    destroy_render_state(gameContext->render_state, gameContext->game_area);
    gameContext->quit_callback(gameContext->game_page);
    // free(gameContext);
    // gameContext = NULL;
}
