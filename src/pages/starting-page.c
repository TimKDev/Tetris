#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "starting-page.h"

#define MAX_SCORES 5

struct ScoreEntry
{
    char name[50];
    int score;
};

static int compare_scores(const void *a, const void *b);
static int read_high_scores(struct ScoreEntry scores[]);

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

    // Replace the mock scores section with:
    struct ScoreEntry scores[MAX_SCORES];
    int num_scores = read_high_scores(scores);

    scores_label = gtk_label_new("High Scores:");
    gtk_box_append(GTK_BOX(box), scores_label);

    // Create a grid for scores
    GtkWidget *scoreGrid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(scoreGrid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(scoreGrid), 100);

    gtk_widget_set_halign(scoreGrid, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(scoreGrid, 10);
    gtk_widget_set_margin_bottom(scoreGrid, 10);

    // Add scores to grid
    for (int i = 0; i < num_scores; i++)
    {
        GtkWidget *scoreName = gtk_label_new(scores[i].name);
        char score_str[20];
        snprintf(score_str, sizeof(score_str), "%d", scores[i].score);
        GtkWidget *scoreValue = gtk_label_new(score_str);

        gtk_widget_set_halign(scoreName, GTK_ALIGN_START);
        gtk_widget_set_halign(scoreValue, GTK_ALIGN_END);

        gtk_grid_attach(GTK_GRID(scoreGrid), scoreName, 0, i, 1, 1);
        gtk_grid_attach(GTK_GRID(scoreGrid), scoreValue, 1, i, 1, 1);
    }

    gtk_box_append(GTK_BOX(box), scoreGrid);

    return box;
}

static int compare_scores(const void *a, const void *b)
{
    const struct ScoreEntry *score_a = (const struct ScoreEntry *)a;
    const struct ScoreEntry *score_b = (const struct ScoreEntry *)b;
    return score_b->score - score_a->score;
}

static int read_high_scores(struct ScoreEntry scores[])
{
    FILE *file = fopen("scores.txt", "r");
    if (file == NULL)
    {
        return 0;
    }

    int count = 0;
    int lowest_score = -1;
    char line[256];

    while (fgets(line, sizeof(line), file))
    {
        char *colon = strchr(line, ':');
        if (colon == NULL)
            continue;

        *colon = '\0';
        int current_score = atoi(colon + 1);

        if (count < MAX_SCORES)
        {
            strncpy(scores[count].name, line, sizeof(scores[count].name) - 1);
            scores[count].score = current_score;
            count++;

            if (lowest_score == -1 || current_score < lowest_score)
            {
                lowest_score = current_score;
            }

            qsort(scores, count, sizeof(struct ScoreEntry), compare_scores);
        }
        else if (current_score > lowest_score)
        {
            strncpy(scores[MAX_SCORES - 1].name, line, sizeof(scores[MAX_SCORES - 1].name) - 1);
            scores[MAX_SCORES - 1].score = current_score;
            qsort(scores, MAX_SCORES, sizeof(struct ScoreEntry), compare_scores);
            lowest_score = scores[MAX_SCORES - 1].score;
        }
    }

    fclose(file);
    return count;
}