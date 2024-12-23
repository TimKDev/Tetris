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

static int y_position = 200;
GtkWidget **moving_blocks = NULL;

GtkWidget *create_game_page(GtkWidget *window)
{
    GtkWidget *box;
    GtkWidget *game_area;
    GtkWidget *side_panel;
    GtkWidget *score_label;
    GtkWidget *next_piece_label;
    static int counter = 0;

    GameConfig *config = load_config_from_file("config/game.json");
    GameData *game_data = initialize(config);
    print_game_data(game_data);

    // Create horizontal box container for main layout
    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    gtk_widget_set_margin_start(box, 20);
    gtk_widget_set_margin_end(box, 20);
    gtk_widget_set_margin_top(box, 20);
    gtk_widget_set_margin_bottom(box, 20);

    // Create game area (left side)
    game_area = gtk_fixed_new();
    gtk_widget_set_size_request(game_area, GAME_WIDTH + 2 * BORDER_WIDTH, GAME_HEIGHT + 2 * BORDER_WIDTH);
    gtk_widget_add_css_class(game_area, "game-area");

    // Make the widget non-expandable
    gtk_widget_set_halign(game_area, GTK_ALIGN_START);
    gtk_widget_set_valign(game_area, GTK_ALIGN_START);
    gtk_widget_set_hexpand(game_area, FALSE);
    gtk_widget_set_vexpand(game_area, FALSE);

    RenderState *render_state = create_render_state();
    GameContext *game_context = (GameContext *)malloc(sizeof(GameContext));
    game_context->game_area = game_area;
    game_context->game_data = game_data;
    game_context->render_state = render_state;
    game_context->config = config;

    // Das macht den Code irgendwie langsam und funktioniert nicht.
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

    // Add main elements to box
    gtk_box_append(GTK_BOX(box), game_area);
    gtk_box_append(GTK_BOX(box), side_panel);

    return box;
}

static gboolean update_game(GameContext *gameContext)
{
    nextMove(gameContext->game_data, gameContext->config);
    render_game_data(gameContext);
    return G_SOURCE_CONTINUE; // Return TRUE to keep the timer running
}

// Callback function for key events
static gboolean on_key_pressed(GtkEventControllerKey *controller,
                               guint keyval,
                               guint keycode,
                               GdkModifierType state,
                               gpointer user_data)
{
    GameContext *context = (GameContext *)user_data;

    switch (keyval)
    {
    case GDK_KEY_Left:
        move_piece_left(context->game_data);
        break;

    case GDK_KEY_Right:
        move_piece_right(context->game_data);
        break;

    case GDK_KEY_Up:
        // Handle up arrow
        printf("Up arrow pressed\n");
        break;

    case GDK_KEY_Down:
        // Handle down arrow
        move_piece_down(context->game_data);
        break;

    default:
        return FALSE; // Event not handled
    }

    // Trigger a re-render if needed
    render_game_data(context);
    return TRUE; // Event handled
}