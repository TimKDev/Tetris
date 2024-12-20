#include "gameConfig.h"

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#define GAME_WIDTH 8
#define GAME_HEIGHT 8
#define BLOCK_SIZE 25

typedef struct
{
    GridValue value;
    Point *blocks;
    int numberOfBlocks;
} Piece;

typedef struct
{
    GridValue fixedBlocks[GAME_WIDTH][GAME_HEIGHT];
    Piece *activePiece;
    Piece *nextPiece;
    int score;
} GameData;

GameData *initialize();

void print_game_data(GameData *game);

void clean_up();

GameData *nextMove();

#endif