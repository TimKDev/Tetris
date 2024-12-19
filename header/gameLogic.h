#include "gameConfig.h"

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#define GAME_WIDTH 8
#define GAME_HEIGHT 8

typedef struct
{
    GridValue fixedBlocks[GAME_WIDTH][GAME_HEIGHT];
    Piece activePiece;
    Piece nextPiece;
    int score;
} GameData;

typedef struct
{
    GridValue value;
    Point *blocks;
    int numberOfBlocks;
} Piece;

GameData *initialize();

void clean_up();

GameData *nextMove();

#endif