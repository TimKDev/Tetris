#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../header/gameLogic.h"

GameData *initialize();
void print_game_data(GameData *game);
void clean_up();
GameData *nextMove();
void print_piece(Piece *piece);
Piece *create_random_piece();
void destory_game_data();

static GameData *gameData;
static GameConfig *config;

GameData *initialize()
{
    srand(time(NULL));
    config = load_config_from_file("config/game.json");
    gameData = (GameData *)calloc(1, sizeof(GameData));

    gameData->activePiece = create_random_piece();
    gameData->nextPiece = create_random_piece();

    return gameData;
}

void print_game_data(GameData *game)
{
    printf("Game State:\n");
    printf("Score: %d\n\n", game->score);

    printf("Fixed Blocks:\n");
    for (int y = 0; y < GAME_HEIGHT; y++)
    {
        for (int x = 0; x < GAME_WIDTH; x++)
        {
            printf("%d ", game->fixedBlocks[x][y]);
        }
        printf("\n");
    }

    printf("\nActive Piece:\n");
    print_piece(game->activePiece);

    printf("\nNext Piece:\n");
    print_piece(game->nextPiece);
}

void clean_up()
{
    // Guard Clause für den Fall, dass initialize noch nicht aufgerufen wurde.
    destroy_game_config(config);
    destory_game_data();
}

GameData *nextMove()
{
    // Guard Clause für den Fall, dass initialize noch nicht aufgerufen wurde.
}

void print_piece(Piece *piece)
{
    printf("Value: %d\n", piece->value);
    for (int i = 0; i < piece->numberOfBlocks; i++)
    {
        printf("Position %d: (%d, %d)\n", i, piece->blocks[i].x, piece->blocks[i].y);
    }
}

Piece *create_random_piece()
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
        blocks[i].x = randomPieceConfig[i].x + xOffset;
        blocks[i].y = randomPieceConfig[i].y + yOffset;
    }

    return result;
}

void destory_game_data()
{
    // Hier kommt die free Logik rein für die GameData
}