#ifndef RENDER_LOGIC_H
#define RENDER_LOGIC_H

#include <gtk/gtk.h>
#include "gameLogic.h"

typedef struct
{
    GtkWidget *block;
    FixedBlockNode *previous;
} FixedBlockNode;

typedef struct
{
    GtkWidget *activeBlockWidgets;
    int numberActiveBlocks;
    FixedBlockNode *fixedBlockWidgets;
} RenderState;

RenderState *create_render_state(void);
void destroy_render_state(RenderState *state);
void render_game_data(GtkWidget *game_area, const GameData *data, RenderState *render_state);

#endif