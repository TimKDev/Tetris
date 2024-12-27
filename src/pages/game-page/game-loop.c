#include "game-loop.h"
#include "game-context.h"
#include "game-logic.h"
#include "render-logic.h"
#include "game-over-dialog.h"

gboolean update_game(GameContext *gameContext)
{
    if (!gameContext->is_paused)
    {
        nextMove(gameContext->game_data, gameContext->config);
        render_game_data(gameContext);
        if (gameContext->game_data->gameOver)
        {
            gameContext->is_paused = true;
            show_game_over_dialog(gameContext);
        }
    }
    return G_SOURCE_CONTINUE;
}
