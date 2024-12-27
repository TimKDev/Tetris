#ifndef RENDER_LOGIC_H
#define RENDER_LOGIC_H

#include <gtk/gtk.h>
#include "game-logic.h"

typedef struct
{
    GtkWidget **activeBlockWidgets;
    int numberActiveBlocks;
    GtkWidget **fixedBlockWidgets;
    int numberFixedBlocks;
    GtkWidget **nextPieceBlockWidgets;
    int numberNextPieceBlockWidgets;
} RenderState;

typedef struct
{
    // Widgets
    GtkWidget *game_area;
    GtkWidget *game_page;
    GtkWidget *window;
    GtkWidget *next_piece_area;
    GtkEventController *key_controller;

    // Game state
    GameData *game_data;
    RenderState *render_state;
    GameConfig *config;
    bool is_paused;

    // Signal handler IDs
    gulong key_controller_handler_id;
    guint update_game_handler_id;

    // Callback
    void (*quit_callback)(GtkWidget *widget);
} GameContext;

RenderState *create_render_state(void);
void destroy_render_state(RenderState *state, GtkWidget *gameArea);
void render_game_data(GameContext *context);

#endif