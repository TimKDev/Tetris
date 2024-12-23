#ifndef RENDER_LOGIC_H
#define RENDER_LOGIC_H

#include <gtk/gtk.h>
#include "gameLogic.h"

typedef struct FixedBlockNode
{
    GtkWidget *block;
    struct FixedBlockNode *previous;
} FixedBlockNode;

typedef struct
{
    GtkWidget **activeBlockWidgets;
    int numberActiveBlocks;
    FixedBlockNode *fixedBlockWidgets;
} RenderState;

typedef struct
{
    GtkWidget *game_area;
    GameData *game_data;
    RenderState *render_state;
    GameConfig *config;
} GameContext;

RenderState *create_render_state(void);
void destroy_render_state(RenderState *state);
void render_game_data(GameContext *context);

#endif