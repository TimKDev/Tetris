#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <glib.h>
#include "../header/gameLogic.h"
#include "../header/blockMovement.h"

#define FALL_FRAME_DURATION 2

void print_game_data(GameData *game);
void print_piece(Piece *piece);
Piece *create_random_piece(GameConfig *config);
void destory_game_data();
void destory_piece(Piece *piece);
int is_active_piece_fixed(GameData *gameData);
void add_active_piece_to_fixed(GameData *gameData);
void create_new_actice_piece(GameData *gameData, GameConfig *config);

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
    for (int y = 0; y < GAME_ROWS; y++)
    {
        for (int x = 0; x < GAME_COLUMNS; x++)
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

void nextMove(GameData *gameData, GameConfig *config)
{
    if (frame_counter % FALL_FRAME_DURATION)
    {
        let_piece_fall_down(gameData);
    }

    if (is_active_piece_fixed(gameData) == 0)
    {
        add_active_piece_to_fixed(gameData);
        create_new_actice_piece(gameData, config);
    }

    frame_counter++;
}

void add_active_piece_to_fixed(GameData *gameData)
{
    for (size_t i = 0; i < gameData->activePiece->numberOfBlocks; i++)
    {
        Point p = gameData->activePiece->blocks[i];
        int col = p.x / BLOCK_SIZE;
        int row = p.y / BLOCK_SIZE;
        gameData->fixedBlocks[row][col] = gameData->activePiece->value;
    }
    destory_piece(gameData->activePiece);
}

void create_new_actice_piece(GameData *gameData, GameConfig *config)
{
    gameData->activePiece = gameData->nextPiece;
    gameData->nextPiece = create_random_piece(config);
}

int is_active_piece_fixed(GameData *gameData)
{
    for (size_t i = 0; i < gameData->activePiece->numberOfBlocks; i++)
    {
        Point p = gameData->activePiece->blocks[i];
        if (p.y + BLOCK_SIZE == GAME_HEIGHT)
        {
            return 0;
        }

        for (size_t row = 0; row < GAME_ROWS; row++)
        {
            for (size_t col = 0; col < GAME_COLUMNS; col++)
            {
                if (gameData->fixedBlocks[row][col] == Empty)
                {
                    continue;
                }

                if (p.x == row * BLOCK_SIZE && p.y + BLOCK_SIZE == row * BLOCK_SIZE)
                {
                    return 0;
                }
            }
        }
    }

    return 1;
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

void destory_piece(Piece *piece)
{
    free(piece->blocks);
    free(piece);
}

void destory_game_data()
{
    // Hier kommt die free Logik rein für die GameData
}