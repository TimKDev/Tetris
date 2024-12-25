#include <gtk/gtk.h>
#include "../header/renderLogic.h"
#include "renderLogic.h"
#include "gameLogic.h"

#define BORDER_BLOCK 2

char *getColorFromGridValue(GridValue value);
GtkWidget *add_block_to_game(GtkWidget *gameArea, int x, int y, GridValue gridValue);
void render_active_piece(GameContext *context);
void render_fixed_blocks(GameContext *context);

RenderState *create_render_state(void)
{
    return (RenderState *)calloc(1, sizeof(RenderState));
}

void destroy_render_state(RenderState *state)
{
}

void render_game_data(GameContext *context)
{
    render_active_piece(context);
    if (context->game_data->numberFixedBlocks != context->render_state->numberFixedBlocks)
    {
        render_fixed_blocks(context);
    }
}

void render_active_piece(GameContext *context)
{
    if (!context || !context->render_state || !context->game_area)
    {
        g_error("Invalid context in render_active_piece");
        return;
    }

    for (int i = 0; i < context->render_state->numberActiveBlocks; i++)
    {
        if (context->render_state->activeBlockWidgets[i])
        {
            GtkWidget *widget = context->render_state->activeBlockWidgets[i];
            gtk_fixed_remove(GTK_FIXED(context->game_area), widget);
            widget = NULL;
        }
    }

    size_t new_number_active_blocks = context->game_data->activePiece->numberOfBlocks;
    size_t old_number_active_blocks = context->render_state->numberActiveBlocks;

    if (old_number_active_blocks < new_number_active_blocks)
    {
        GtkWidget **new_widgets = (GtkWidget **)realloc(context->render_state->activeBlockWidgets, new_number_active_blocks * sizeof(GtkWidget *));

        if (new_widgets == NULL)
        {
            g_error("Failed to allocate memory for widgets");
            return;
        }

        context->render_state->activeBlockWidgets = new_widgets;
        context->render_state->numberActiveBlocks = new_number_active_blocks;
    }

    for (int i = 0; i < new_number_active_blocks; i++)
    {
        Piece *activePiece = context->game_data->activePiece;
        GtkWidget *block = add_block_to_game(context->game_area, activePiece->blocks[i].x, activePiece->blocks[i].y, activePiece->value);
        context->render_state->activeBlockWidgets[i] = block;
    }
}

void render_fixed_blocks(GameContext *context)
{
    if (!context || !context->render_state || !context->game_area)
    {
        g_error("Invalid context in render_fixed_blocks");
        return;
    }

    for (int i = 0; i < context->render_state->numberFixedBlocks; i++)
    {
        if (context->render_state->fixedBlockWidgets[i])
        {
            GtkWidget *widget = context->render_state->fixedBlockWidgets[i];
            gtk_fixed_remove(GTK_FIXED(context->game_area), widget);
            context->render_state->fixedBlockWidgets[i] = NULL;
        }
    }

    size_t newSize = context->game_data->numberFixedBlocks;
    GtkWidget **new_widgets = (GtkWidget **)realloc(context->render_state->fixedBlockWidgets, newSize * sizeof(GtkWidget *));

    if (new_widgets == NULL)
    {
        g_error("Failed to allocate memory for widgets");
        return;
    }

    if (newSize > context->render_state->numberFixedBlocks)
    {
        for (size_t i = context->render_state->numberFixedBlocks; i < newSize; i++)
        {
            new_widgets[i] = NULL;
        }
    }

    context->render_state->fixedBlockWidgets = new_widgets;
    context->render_state->numberFixedBlocks = newSize;

    int counter = 0;
    for (size_t row = 0; row < GAME_ROWS; row++)
    {
        for (size_t col = 0; col < GAME_COLUMNS; col++)
        {
            GridValue fixedValue = context->game_data->fixedBlocks[row][col];
            if (fixedValue == Empty)
            {
                continue;
            }
            if (counter < newSize)
            {
                GtkWidget *block = add_block_to_game(context->game_area, col * BLOCK_SIZE, row * BLOCK_SIZE, fixedValue);
                context->render_state->fixedBlockWidgets[counter] = block;
                counter++;
            }
            else
            {
                g_error("Missmatch between number of fixed blocks and array of fixed blocks.");
            }
        }
    }
}

GtkWidget *add_block_to_game(GtkWidget *gameArea, int x, int y, GridValue gridValue)
{
    GtkWidget *block = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(block, BLOCK_SIZE, BLOCK_SIZE);
    gtk_widget_add_css_class(block, "block");
    gtk_widget_add_css_class(block, getColorFromGridValue(gridValue));
    gtk_fixed_put(GTK_FIXED(gameArea), block, x, y);

    return block;
}

// Dies ist durch die Config eigentlich überflüssig, da hier auch der CSS Class Name gefunden werden kann.
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
        printf("Unknown color with value %d", value);
        return "";
    }
}
