#include "game-context.h"
#include "game-logic.h"
#include "game-config.h"
#include "game-ui.h"

GameContext *create_game_context(GtkWidget *gamePage, GtkWidget *gameArea, void (*quitCallback)(GtkWidget *widget))
{
    GameContext *game_context = (GameContext *)malloc(sizeof(GameContext));

    // Initialize all fields to NULL/0
    memset(game_context, 0, sizeof(GameContext));

    // Store widgets
    game_context->game_area = gameArea;
    game_context->game_page = gamePage;
    game_context->window = GTK_WIDGET(gtk_widget_get_root(gameArea));

    // Initialize game state
    game_context->config = load_config_from_file("config/game.json");
    game_context->game_data = create_game_data(game_context->config);
    game_context->render_state = create_render_state();
    game_context->quit_callback = quitCallback;

    return game_context;
}

void restart_game(GameContext *context)
{
    if (context == NULL)
    {
        return;
    }

    GtkWidget *gameArea = context->game_area;

    if (context->render_state)
    {
        destroy_render_state(context->render_state, gameArea);
        context->render_state = NULL;
    }

    if (context->game_data)
    {
        destory_game_data(context->game_data);
        context->game_data = NULL;
    }

    if (context->config)
    {
        destroy_game_config(context->config);
        context->config = NULL;
    }

    context->config = load_config_from_file("config/game.json");
    context->game_data = create_game_data(context->config);
    context->render_state = create_render_state();
    context->is_paused = false;

    render_game_data(context);
}

void quit_game(GameContext *gameContext)
{
    if (!gameContext)
    {
        return;
    }

    destroy_game_page_ui_elements(gameContext);

    if (gameContext->render_state)
    {
        destroy_render_state(gameContext->render_state, gameContext->game_area);
    }
    if (gameContext->game_data)
    {
        destory_game_data(gameContext->game_data);
    }
    if (gameContext->config)
    {
        destroy_game_config(gameContext->config);
    }

    if (gameContext->quit_callback)
    {
        gameContext->quit_callback(gameContext->game_page);
    }

    free(gameContext);
}