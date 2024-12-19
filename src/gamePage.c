#include "gamePage.h"
#include "gameLogic.h"

#define BLOCK_SIZE 25

typedef struct
{
    int x;
    int y;
} Position;

static gboolean update_counter(gpointer data);

GtkWidget **create_tetromino(GtkWidget *game_area, Position pos, const char *color)
{
    // Allocate new memory for each tetromino
    GtkWidget **blocks = g_new(GtkWidget *, 4);

    for (int i = 0; i < 4; i++)
    {
        blocks[i] = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_widget_set_size_request(blocks[i], BLOCK_SIZE, BLOCK_SIZE);
        gtk_widget_add_css_class(blocks[i], "block");
        gtk_widget_add_css_class(blocks[i], color);
    }

    gtk_fixed_put(GTK_FIXED(game_area), blocks[0], pos.x, pos.y);
    gtk_fixed_put(GTK_FIXED(game_area), blocks[1], pos.x + BLOCK_SIZE, pos.y);
    gtk_fixed_put(GTK_FIXED(game_area), blocks[2], pos.x + 2 * BLOCK_SIZE, pos.y);
    gtk_fixed_put(GTK_FIXED(game_area), blocks[3], pos.x + 2 * BLOCK_SIZE, pos.y + BLOCK_SIZE);

    return blocks;
}

// Es wird eine Methode benötigt, die alle Blöcke des gesamten Games nach einem Frame löscht, sonst Memory Leak
void destroy_tetromino(GtkWidget **blocks)
{
    g_free(blocks);
}

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
    g_timeout_add(100, G_SOURCE_FUNC(update_counter), game_area);

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

static gboolean update_counter(gpointer game_area)
{
    GtkWidget *game = (GtkWidget *)game_area;
    y_position += 1;

    if (moving_blocks != NULL)
    {
        for (int i = 0; i < 4; i++)
        {
            gtk_fixed_remove(GTK_FIXED(game), moving_blocks[i]);
        }
        destroy_tetromino(moving_blocks);
    }

    moving_blocks = create_tetromino(game_area, (Position){50, y_position}, "green");
    return G_SOURCE_CONTINUE; // Return TRUE to keep the timer running
}
