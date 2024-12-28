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
static bool is_row_full(GridValue row[GAME_COLUMNS]);
static int get_full_fixed_row_number(GameData *gameData);
static void remove_line(GameData *gameData, int lineNumber);
static int calculate_score(GameData *gameData, GameConfig *config, int lineNumber);

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
        int fullLineNumber = get_full_fixed_row_number(gameData);
        while (fullLineNumber != -1)
        {
            gameData->score += calculate_score(gameData, config, fullLineNumber);
            remove_line(gameData, fullLineNumber);
            fullLineNumber = get_full_fixed_row_number(gameData);
        }

        create_new_actice_piece(gameData, config);
        statusOfNewPiece = get_active_piece_status(gameData);
    }
    if (activePieceStatus == GameOver || statusOfNewPiece == GameOver)
    {
        gameData->gameOver = true;
    }

    frame_counter++;
}

static int calculate_score(GameData *gameData, GameConfig *config, int lineNumber)
{
    return 10;
}

static void remove_line(GameData *gameData, int lineNumber)
{
    for (int row = lineNumber; row >= 0; row--)
    {
        for (size_t col = 0; col < GAME_COLUMNS; col++)
        {
            gameData->fixedBlocks[row][col] = row == 0 ? Empty : gameData->fixedBlocks[row - 1][col];
        }
    }
}

static int get_full_fixed_row_number(GameData *gameData)
{
    for (int i = 0; i < GAME_ROWS; i++)
    {
        if (is_row_full(gameData->fixedBlocks[i]))
        {
            return i;
        }
    }

    return -1;
}

static bool is_row_full(GridValue row[GAME_COLUMNS])
{
    for (size_t i = 0; i < GAME_COLUMNS; i++)
    {
        if (row[i] == Empty)
        {
            return false;
        }
    }
    return true;
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
