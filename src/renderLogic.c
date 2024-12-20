#include <gtk/gtk.h>
#include "../header/renderLogic.h"
#include "renderLogic.h"
#include "gameLogic.h"

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
        Piece *activePiece = data->activePiece;
        GtkWidget *block = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_widget_set_size_request(block, BLOCK_SIZE, BLOCK_SIZE);
        gtk_widget_add_css_class(block, "block");
        gtk_widget_add_css_class(block, getColorFromGridValue(activePiece->value));
        gtk_fixed_put(GTK_FIXED(game_area), block, activePiece->blocks[i].x * BLOCK_SIZE, activePiece->blocks[i].y * BLOCK_SIZE);
        // Implementiere das Rendering für die Active Piece und danach überrprüfe, ob zu den fixed Pieces neue hinzugekommen sind,
        // d.h. ob der Pointer der Reversed Linked List im RenderState anders ist als vorher.
    }
}

char *getColorFromGridValue(GridValue value)
{
    switch (value)
    {
    case Blue:
        return "blue";
    case Red:
        return "red";
    case Yellow:
        return "yellow";
    case Green:
        return "green";
    default:
        printf("Unknown color");
        return "";
    }
}
