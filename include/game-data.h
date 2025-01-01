#pragma once

#include "game-config.h"
#include "stdbool.h"

#define GAME_COLUMNS 10
#define GAME_ROWS 20
#define GAME_WIDTH (GAME_COLUMNS * BLOCK_SIZE)
#define GAME_HEIGHT (GAME_ROWS * BLOCK_SIZE)
#define BLOCK_SIZE 25

#define X_OFFSET 4
#define Y_OFFSET 0

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
    char *playerName;
    bool gameOver;
} GameData;

GameData *create_game_data(GameConfig *config, char *playerName);

void print_game_data(GameData *game);

void destory_game_data(GameData *gameData);

void destory_piece(Piece *piece);

Piece *create_random_piece(GameConfig *config);
