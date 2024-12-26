#include "gameLogic.h"
#include "blockMovement.h"

#define FALL_VELOCITY 1

void y_move_piece_by_offset(GameData *gameData, int yOffset);
void x_move_piece_by_offset(GameData *gameData, int xOffset);
int is_x_position_valid(GameData *gameData, int xOffset);
int get_allowed_y_offset(GameData *gameData, int yOffset);

void move_piece_down(GameData *gameData)
{
    int allowedYOffset = get_allowed_y_offset(gameData, BLOCK_SIZE);
    y_move_piece_by_offset(gameData, allowedYOffset);
}

void let_piece_fall_down(GameData *gameData)
{
    int allowedYOffset = get_allowed_y_offset(gameData, FALL_VELOCITY);
    y_move_piece_by_offset(gameData, allowedYOffset);
}

void move_piece_right(GameData *gameData)
{
    if (is_x_position_valid(gameData, BLOCK_SIZE) != 0)
    {
        return;
    }
    x_move_piece_by_offset(gameData, BLOCK_SIZE);
}

void move_piece_left(GameData *gameData)
{
    if (is_x_position_valid(gameData, -BLOCK_SIZE) != 0)
    {
        return;
    }
    x_move_piece_by_offset(gameData, -BLOCK_SIZE);
}

void y_move_piece_by_offset(GameData *gameData, int yOffset)
{
    for (int i = 0; i < gameData->activePiece->numberOfBlocks; i++)
    {
        gameData->activePiece->blocks[i].y += yOffset;
    }
}

void x_move_piece_by_offset(GameData *gameData, int xOffset)
{
    for (int i = 0; i < gameData->activePiece->numberOfBlocks; i++)
    {
        gameData->activePiece->blocks[i].x += xOffset;
    }
}

int is_x_position_valid(GameData *gameData, int xOffset)
{
    for (size_t i = 0; i < gameData->activePiece->numberOfBlocks; i++)
    {
        Point p = gameData->activePiece->blocks[i];
        if (p.x + xOffset < 0 || GAME_WIDTH < p.x + BLOCK_SIZE + xOffset)
        {
            return 1;
        }
        for (size_t row = 0; row < GAME_ROWS; row++)
        {
            for (size_t col = 0; col < GAME_COLUMNS; col++)
            {
                if (gameData->fixedBlocks[row][col] == Empty)
                {
                    continue;
                }

                if (row * BLOCK_SIZE <= p.y && p.y <= row * (BLOCK_SIZE + 1) && p.x + BLOCK_SIZE >= col * BLOCK_SIZE)
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

int get_allowed_y_offset(GameData *gameData, int yOffset)
{
    for (size_t i = 0; i < gameData->activePiece->numberOfBlocks; i++)
    {
        Point p = gameData->activePiece->blocks[i];
        if (p.y + yOffset < 0)
        {
            return 0;
        }

        if (GAME_HEIGHT < p.y + BLOCK_SIZE + yOffset)
        {
            return GAME_HEIGHT - p.y - BLOCK_SIZE;
        }

        for (size_t row = 0; row < GAME_ROWS; row++)
        {
            for (size_t col = 0; col < GAME_COLUMNS; col++)
            {
                if (gameData->fixedBlocks[row][col] == Empty)
                {
                    continue;
                }

                if (p.x == col * BLOCK_SIZE && p.y + BLOCK_SIZE + yOffset >= row * BLOCK_SIZE)
                {
                    int correctionValue = row * BLOCK_SIZE - p.y - BLOCK_SIZE;
                    if (correctionValue < 0 && yOffset < 0 || correctionValue > 0 && yOffset > 0 || correctionValue == 0)
                    {
                        // This check is important in order for tetris caves to work. Otherwise the block is set always on top and jumps.
                        return correctionValue;
                    }
                }
            }
        }
    }

    return yOffset;
}
