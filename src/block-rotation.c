#include "block-rotation.h"
#include "game-data.h"

static void rotate_piece_clockwise(Piece *piece);

void rotation_active_piece_clockwise(GameData *gameData)
{
    //Check that rotation is allowed
    rotate_piece_clockwise(gameData->activePiece);
}

static void rotate_piece_clockwise(Piece *piece)
{
    // The convention is that the first block is the fix point of the rotation.
    const int x_fix = piece->blocks[0].x;
    const int y_fix = piece->blocks[0].y;
    for (size_t i = 1; i < piece->numberOfBlocks; i++)
    {
        const int x_new = x_fix - y_fix + piece->blocks[i].y;
        const int y_new = y_fix + x_fix - piece->blocks[i].x;
        piece->blocks[i].x = x_new;
        piece->blocks[i].y = y_new;
    }
}