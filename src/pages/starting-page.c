#include <string.h>
#include "starting-page.h"

struct ScoreEntry
{
    char name[50];
    int score;
};

GtkWidget *create_starting_page(void (*startCallback)(GtkWidget *widget, gpointer data), void (*helpCallback)(GtkWidget *widget, gpointer data))
{
    GtkWidget *box;
    GtkWidget *title_label;
    GtkWidget *play_button;
    GtkWidget *help_button;
    GtkWidget *scores_label;
    GtkWidget *button_box;

    // Create vertical box container
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_start(box, 20);
    gtk_widget_set_margin_end(box, 20);
    gtk_widget_set_margin_top(box, 20);
    gtk_widget_set_margin_bottom(box, 20);

    // Create and add title
    title_label = gtk_label_new("Welcome to Tetris");
    gtk_widget_add_css_class(title_label, "title");

    button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    gtk_widget_set_halign(button_box, GTK_ALIGN_CENTER);

    play_button = gtk_button_new_with_label("Play");
    help_button = gtk_button_new_with_label("Help");
    gtk_box_append(GTK_BOX(button_box), play_button);
    gtk_box_append(GTK_BOX(button_box), help_button);
    gtk_widget_set_margin_top(button_box, 10);
    gtk_widget_set_margin_bottom(button_box, 10);

    g_signal_connect(play_button, "clicked", G_CALLBACK(startCallback), NULL);
    g_signal_connect(help_button, "clicked", G_CALLBACK(helpCallback), NULL);

    gtk_box_append(GTK_BOX(box), title_label);
    gtk_box_append(GTK_BOX(box), button_box);

    // Create high scores section
    struct ScoreEntry scores[2];
    strncpy(scores[0].name, "John", sizeof(scores[0].name) - 1);
    scores[0].score = 121523;
    strncpy(scores[1].name, "Coding Challenges", sizeof(scores[1].name) - 1);
    scores[1].score = 110323;

    scores_label = gtk_label_new("High Scores:");
    gtk_box_append(GTK_BOX(box), scores_label);

    // Create a grid for scores
    GtkWidget *scoreGrid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(scoreGrid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(scoreGrid), 100);

    // Center the grid horizontally
    gtk_widget_set_halign(scoreGrid, GTK_ALIGN_CENTER);

    // Optional: Add some margin for better spacing
    gtk_widget_set_margin_top(scoreGrid, 10);
    gtk_widget_set_margin_bottom(scoreGrid, 10);

    // Add scores to grid
    int NUM_SCORES = (sizeof(scores) / sizeof(scores[0]));

    for (int i = 0; i < NUM_SCORES; i++)
    {
        GtkWidget *scoreName = gtk_label_new(scores[i].name);
        char score_str[20];
        snprintf(score_str, sizeof(score_str), "%d", scores[i].score);
        GtkWidget *scoreValue = gtk_label_new(score_str);

        // Left-align name, right-align score
        gtk_widget_set_halign(scoreName, GTK_ALIGN_START);
        gtk_widget_set_halign(scoreValue, GTK_ALIGN_END);

        // Attach to grid: widget, column, row, width, height
        gtk_grid_attach(GTK_GRID(scoreGrid), scoreName, 0, i, 1, 1);
        gtk_grid_attach(GTK_GRID(scoreGrid), scoreValue, 1, i, 1, 1);
    }

    // Add the grid to the main box
    gtk_box_append(GTK_BOX(box), scoreGrid);

    return box;
}