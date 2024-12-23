#include "gameConfig.h"

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#define GAME_COLUMNS 8
#define GAME_ROWS 8
#define GAME_WIDTH 300
#define GAME_HEIGHT 400
#define BLOCK_SIZE 25

typedef struct
{
    GridValue value;
    Point *blocks;
    int numberOfBlocks;
} Piece;

typedef struct
{
    GridValue fixedBlocks[GAME_COLUMNS][GAME_ROWS];
    Piece *activePiece;
    Piece *nextPiece;
    int score;
} GameData;

GameData *initialize(GameConfig *config);

void print_game_data(GameData *game);

void clean_up();

void nextMove(GameData *gameData, GameConfig *config);

void move_piece_left(GameData *gameData);

void move_piece_right(GameData *gameData);

void move_piece_down(GameData *gameData);

#endif