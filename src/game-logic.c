#include <glib.h>
#include "game-logic.h"
#include "block-movement.h"
#include "game-data.h"

#define FALL_FRAME_DURATION 2

typedef enum
{
    Free,
    Fixed,
    GameOver
} ActivePieceStatus;

static ActivePieceStatus get_active_piece_status(GameData *gameData);
static void create_new_actice_piece(GameData *gameData, GameConfig *config);
static void add_active_piece_to_fixed(GameData *gameData);

int frame_counter = 0;

void nextMove(GameData *gameData, GameConfig *config)
{
    if (frame_counter % FALL_FRAME_DURATION)
    {
        let_piece_fall_down(gameData);
    }

    ActivePieceStatus activePieceStatus = get_active_piece_status(gameData);
    ActivePieceStatus statusOfNewPiece = Free;
    if (activePieceStatus == Fixed)
    {
        add_active_piece_to_fixed(gameData);
        create_new_actice_piece(gameData, config);
        statusOfNewPiece = get_active_piece_status(gameData);
    }
    if (activePieceStatus == GameOver || statusOfNewPiece == GameOver)
    {
        gameData->gameOver = true;
        print_game_data(gameData);
        g_print("GameOver");
    }

    frame_counter++;
}

static void add_active_piece_to_fixed(GameData *gameData)
{
    for (size_t i = 0; i < gameData->activePiece->numberOfBlocks; i++)
    {
        Point p = gameData->activePiece->blocks[i];
        int col = p.x / BLOCK_SIZE;
        int row = p.y / BLOCK_SIZE;
        gameData->fixedBlocks[row][col] = gameData->activePiece->value;
        gameData->numberFixedBlocks++;
    }
    destory_piece(gameData->activePiece);
}

static void create_new_actice_piece(GameData *gameData, GameConfig *config)
{
    gameData->activePiece = gameData->nextPiece;
    gameData->nextPiece = create_random_piece(config);
}

static ActivePieceStatus get_active_piece_status(GameData *gameData)
{
    for (size_t i = 0; i < gameData->activePiece->numberOfBlocks; i++)
    {
        Point p = gameData->activePiece->blocks[i];
        if (p.y + BLOCK_SIZE == GAME_HEIGHT)
        {
            return Fixed;
        }

        for (size_t row = 0; row < GAME_ROWS; row++)
        {
            for (size_t col = 0; col < GAME_COLUMNS; col++)
            {
                if (gameData->fixedBlocks[row][col] == Empty)
                {
                    continue;
                }

                if (p.x == col * BLOCK_SIZE && p.y + BLOCK_SIZE == row * BLOCK_SIZE)
                {
                    return Fixed;
                }

                if (p.x == col * BLOCK_SIZE && p.y == row * BLOCK_SIZE)
                {
                    return GameOver;
                }
            }
        }
    }

    return Free;
}
