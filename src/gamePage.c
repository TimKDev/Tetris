#include "gamePage.h"
#include "gameLogic.h"

static gboolean update_game(gpointer data);

static int y_position = 200;
GtkWidget **moving_blocks = NULL;

GtkWidget *create_game_page(void)
{
    GtkWidget *box;
    GtkWidget *game_area;
    GtkWidget *side_panel;
    GtkWidget *score_label;
    GtkWidget *next_piece_label;
    static int counter = 0;

    GameData *gameData = initialize();
    print_game_data(gameData);

    // Create horizontal box container for main layout
    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    gtk_widget_set_margin_start(box, 20);
    gtk_widget_set_margin_end(box, 20);
    gtk_widget_set_margin_top(box, 20);
    gtk_widget_set_margin_bottom(box, 20);

    // Create game area (left side)
    game_area = gtk_fixed_new();
    gtk_widget_set_size_request(game_area, 300, 400);
    gtk_widget_add_css_class(game_area, "game-area");

    // Create a timer function that will be called every second
    g_timeout_add(100, G_SOURCE_FUNC(update_game), game_area, gameData);

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

static gboolean update_game(gpointer game_area, GameData gameData)
{
    GtkWidget *game = (GtkWidget *)game_area;
    return G_SOURCE_CONTINUE; // Return TRUE to keep the timer running
}
