#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../header/gameLogic.h"

#define FallVelocity = 0.1f

GameData *initialize();
void print_game_data(GameData *game);
void clean_up();
GameData *nextMove();
void print_piece(Piece *piece);
Piece *create_random_piece();
void destory_game_data();

GameData *initialize(GameConfig *config)
{
    srand(time(NULL));
    GameData *gameData = (GameData *)calloc(1, sizeof(GameData));

    gameData->activePiece = create_random_piece(config);
    gameData->nextPiece = create_random_piece(config);

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
    destory_game_data();
}

GameData *nextMove(GameData gameData)
{
    // Guard Clause für den Fall, dass initialize noch nicht aufgerufen wurde.
    //
}

void move_piece_down()
{
    
}

void print_piece(Piece *piece)
{
    printf("Value: %d\n", piece->value);
    for (int i = 0; i < piece->numberOfBlocks; i++)
    {
        printf("Position %d: (%d, %d)\n", i, piece->blocks[i].x, piece->blocks[i].y);
    }
}

Piece *create_random_piece(GameConfig *config)
{
    int randomColorPosition = rand() % config->colorsCount;
    printf("randomColorPostion: %d", randomColorPosition);
    ColorConfig randomColorConfig = config->colors[randomColorPosition];
    printf("randomColor: %d", randomColorConfig.key);
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