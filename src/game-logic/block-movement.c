#include "game-logic.h"
#include "block-movement.h"

#define FALL_VELOCITY 1

static void y_move_piece_by_offset(GameData *gameData, int yOffset);
static void x_move_piece_by_offset(GameData *gameData, int xOffset);
static bool is_x_position_valid(GameData *gameData, int xOffset);
static int get_allowed_y_offset(GameData *gameData, int yOffset);
static bool is_point_in_area(int x, int y, int row, int col);

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
    if (is_x_position_valid(gameData, BLOCK_SIZE))
    {
        x_move_piece_by_offset(gameData, BLOCK_SIZE);
    }
}

void move_piece_left(GameData *gameData)
{
    if (is_x_position_valid(gameData, -BLOCK_SIZE))
    {
        x_move_piece_by_offset(gameData, -BLOCK_SIZE);
    }
}

bool is_block_in_area(int x, int y, int row, int col)
{
    return is_point_in_area(x, y, row, col) || is_point_in_area(x + BLOCK_SIZE - 1, y, row, col) || is_point_in_area(x, y + BLOCK_SIZE - 1, row, col) || is_point_in_area(x + BLOCK_SIZE - 1, y + BLOCK_SIZE - 1, row, col);
}

static void y_move_piece_by_offset(GameData *gameData, int yOffset)
{
    for (int i = 0; i < gameData->activePiece->numberOfBlocks; i++)
    {
        gameData->activePiece->blocks[i].y += yOffset;
    }
}

static void x_move_piece_by_offset(GameData *gameData, int xOffset)
{
    for (int i = 0; i < gameData->activePiece->numberOfBlocks; i++)
    {
        gameData->activePiece->blocks[i].x += xOffset;
    }
}

static bool is_x_position_valid(GameData *gameData, int xOffset)
{
    for (size_t i = 0; i < gameData->activePiece->numberOfBlocks; i++)
    {
        Point p = gameData->activePiece->blocks[i];
        if (p.x + xOffset < 0 || GAME_WIDTH < p.x + BLOCK_SIZE + xOffset)
        {
            return false;
        }
        for (size_t row = 0; row < GAME_ROWS; row++)
        {
            for (size_t col = 0; col < GAME_COLUMNS; col++)
            {
                if (gameData->fixedBlocks[row][col] == Empty)
                {
                    continue;
                }

                if (is_block_in_area(p.x + xOffset, p.y, row, col))
                {
                    return false;
                }
            }
        }
    }

    return true;
}

static bool is_point_in_area(int x, int y, int row, int col)
{
    bool x_in_area = col * BLOCK_SIZE <= x && x < (col + 1) * BLOCK_SIZE;
    bool y_in_area = row * BLOCK_SIZE <= y && y < (row + 1) * BLOCK_SIZE;

    return x_in_area && y_in_area;
}

static int get_allowed_y_offset(GameData *gameData, int yOffset)
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
