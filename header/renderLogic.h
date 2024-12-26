#ifndef RENDER_LOGIC_H
#define RENDER_LOGIC_H

#include <gtk/gtk.h>
#include "gameLogic.h"

typedef struct
{
    GtkWidget **activeBlockWidgets;
    int numberActiveBlocks;
    GtkWidget **fixedBlockWidgets;
    int numberFixedBlocks;
} RenderState;

typedef struct
{
    GtkWidget *game_area;
    GameData *game_data;
    RenderState *render_state;
    GameConfig *config;
    bool is_paused;
} GameContext;

RenderState *create_render_state(void);
void destroy_render_state(RenderState *state, GtkWidget *gameArea);
void render_game_data(GameContext *context);

#endif