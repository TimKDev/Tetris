#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "game-logic.h"
#include <glib.h>

void print_game_data(GameData *game);
static void print_piece(Piece *piece);

GameData *create_game_data(GameConfig *config)
{
    srand(time(NULL));
    GameData *gameData = (GameData *)calloc(1, sizeof(GameData));

    gameData->activePiece = create_random_piece(config);
    gameData->nextPiece = create_random_piece(config);
    gameData->gameOver = false;

    return gameData;
}

void destory_game_data(GameData *gameData)
{
    if (gameData == NULL)
    {
        return;
    }
    free(gameData->activePiece);
    free(gameData->nextPiece);
    free(gameData);
    gameData = NULL;
}

void destory_piece(Piece *piece)
{
    free(piece->blocks);
    free(piece);
}

void print_game_data(GameData *game)
{
    g_print("Game State:\n");
    g_print("Score: %d\n\n", game->score);

    g_print("Fixed Blocks:\n");
    for (int y = 0; y < GAME_ROWS; y++)
    {
        for (int x = 0; x < GAME_COLUMNS; x++)
        {
            g_print("%d ", game->fixedBlocks[y][x]);
        }
        g_print("\n");
    }

    printf("\nActive Piece:\n");
    print_piece(game->activePiece);

    g_print("\nNext Piece:\n");
    print_piece(game->nextPiece);
}

Piece *create_random_piece(GameConfig *config)
{
    int randomColorPosition = rand() % config->colorsCount;
    ColorConfig randomColorConfig = config->colors[randomColorPosition];
    int randomPiecePosition = rand() % config->piecesCount;
    Point *randomPieceConfig = config->pieces[randomPiecePosition];
    int numberOfBlocksInRandomPiece = config->pieceSizes[randomPiecePosition];

    Piece *result = (Piece *)malloc(sizeof(Piece));
    result->value = randomColorConfig.key;
    Point *blocks = (Point *)malloc(numberOfBlocksInRandomPiece * sizeof(Point));
    result->numberOfBlocks = numberOfBlocksInRandomPiece;
    result->blocks = blocks;

    int xOffset = 4;
    int yOffset = 0;
    for (int i = 0; i < numberOfBlocksInRandomPiece; i++)
    {
        blocks[i].x = (randomPieceConfig[i].x + xOffset) * BLOCK_SIZE;
        blocks[i].y = (randomPieceConfig[i].y + yOffset) * BLOCK_SIZE;
    }

    return result;
}

static void print_piece(Piece *piece)
{
    g_print("Value: %d\n", piece->value);
    for (int i = 0; i < piece->numberOfBlocks; i++)
    {
        g_print("Position %d: (%d, %d)\n", i, piece->blocks[i].x, piece->blocks[i].y);
    }
}
