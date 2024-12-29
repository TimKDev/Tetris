#include "game-loop.h"
#include "game-context.h"
#include "game-logic.h"
#include "render-logic.h"
#include "game-page.h"
#include "game-over-dialog.h"
#include <stdio.h>

static void save_score_to_file(const char *player_name, int score);

gboolean update_game(GameContext *gameContext)
{
    if (!gameContext->is_paused)
    {
        nextMove(gameContext->game_data, gameContext->config);
        render_game_data(gameContext);
        update_score_display(gameContext);

        if (gameContext->game_data->gameOver)
        {
            gameContext->is_paused = true;
            save_score_to_file(gameContext->game_data->playerName, gameContext->game_data->score);
            show_game_over_dialog(gameContext);
        }
    }
    return G_SOURCE_CONTINUE;
}

static void save_score_to_file(const char *player_name, int score)
{
    FILE *file = fopen("scores.txt", "a");
    if (file == NULL)
    {
        g_warning("Could not open scores file for writing");
        return;
    }

    fprintf(file, "%s:%d\n", player_name, score);
    fclose(file);
}
