#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../header/gameLogic.h"

#define FALL_VELOCITY 1
#define FALL_FRAME_DURATION 2

GameData *initialize();
void print_game_data(GameData *game);
void clean_up();
void nextMove();
void print_piece(Piece *piece);
Piece *create_random_piece();
void move_piece_down(GameData *gameData, int yOffset);
void destory_game_data();
void move_piece_left(GameData *gameData);
void move_piece_right(GameData *gameData);

int frame_counter = 0;

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
    destory_game_data();
}

void nextMove(GameData *gameData)
{
    if (frame_counter % FALL_FRAME_DURATION)
    {
        move_piece_down(gameData, FALL_VELOCITY);
    }

    frame_counter++;
}

void move_piece_down(GameData *gameData, int yOffset)
{
    for (int i = 0; i < gameData->activePiece->numberOfBlocks; i++)
    {
        gameData->activePiece->blocks[i].y += yOffset;
    }
}

void move_piece_right(GameData *gameData)
{
    for (int i = 0; i < gameData->activePiece->numberOfBlocks; i++)
    {
        gameData->activePiece->blocks[i].x += BLOCK_SIZE;
    }
}

void move_piece_left(GameData *gameData)
{
    for (int i = 0; i < gameData->activePiece->numberOfBlocks; i++)
    {
        gameData->activePiece->blocks[i].x -= BLOCK_SIZE;
    }
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
        blocks[i].x = (randomPieceConfig[i].x + xOffset) * BLOCK_SIZE;
        blocks[i].y = (randomPieceConfig[i].y + yOffset) * BLOCK_SIZE;
    }

    return result;
}

void destory_game_data()
{
    // Hier kommt die free Logik rein für die GameData
}