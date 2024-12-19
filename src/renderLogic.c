#include <gtk/gtk.h>
#include "../header/renderLogic.h"
#include "renderLogic.h"

GtkWidget *render_game_data(GtkWidget *game_area, GameData data)
{ // Allocate new memory for each tetromino
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

RenderState *create_render_state(void)
{
    return (RenderState *)calloc(1, sizeof(RenderState));
}

void destroy_render_state(RenderState *state)
{
}

void render_game_data(GtkWidget *game_area, const GameData *data, RenderState *render_state)
{
    for (int i = 0; i < render_state->numberActiveBlocks; i++)
    {
        gtk_fixed_remove(GTK_FIXED(game_area), &render_state->activeBlockWidgets[i]);
        free(&render_state->activeBlockWidgets[i]);
    }

    for (int i = 0; i < data->activePiece->numberOfBlocks; i++)
    {
        // Implementiere das Rendering für die Active Piece und danach überrprüfe, ob zu den fixed Pieces neue hinzugekommen sind,
        // d.h. ob der Pointer der Reversed Linked List im RenderState anders ist als vorher.
    }
}
