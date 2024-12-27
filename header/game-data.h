#include "game-config.h"
#include "stdbool.h"

#ifndef GAME_DATA_H
#define GAME_DATA_H

#define GAME_COLUMNS 8
#define GAME_ROWS 14
#define GAME_WIDTH (GAME_COLUMNS * BLOCK_SIZE)
#define GAME_HEIGHT (GAME_ROWS * BLOCK_SIZE)
#define BLOCK_SIZE 25

typedef struct
{
    GridValue value;
    Point *blocks;
    int numberOfBlocks;
} Piece;

typedef struct
{
    GridValue fixedBlocks[GAME_ROWS][GAME_COLUMNS];
    int numberFixedBlocks;
    Piece *activePiece;
    Piece *nextPiece;
    int score;
    bool gameOver;
} GameData;

GameData *create_game_data(GameConfig *config);

void print_game_data(GameData *game);

void destory_game_data(GameData *gameData);

void destory_piece(Piece *piece);

Piece *create_random_piece(GameConfig *config);

#endif