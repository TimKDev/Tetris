#include "block-rotation.h"
#include "game-data.h"
#include "block-movement.h"

typedef struct
{
    Point *blocks;
    int numberOfBlocks;
} BlocksArray;

static void rotate_piece_clockwise(Piece *piece);
static BlocksArray *compute_new_blocks_after_clockwise_rotation(Piece *piece);
static bool is_clockwise_rotation_valid(GameData *gameData, BlocksArray *blocksToCheck);
static void destroy_blocks_array(BlocksArray *blocksArray);

void rotation_active_piece_clockwise(GameData *gameData)
{
    BlocksArray *rotatedBlocks = compute_new_blocks_after_clockwise_rotation(gameData->activePiece);
    if (is_clockwise_rotation_valid(gameData, rotatedBlocks))
    {
        rotate_piece_clockwise(gameData->activePiece);
    }
    destroy_blocks_array(rotatedBlocks);
}

static bool is_clockwise_rotation_valid(GameData *gameData, BlocksArray *blocksToCheck)
{
    for (size_t i = 0; i < blocksToCheck->numberOfBlocks; i++)
    {
        Point block = blocksToCheck->blocks[i];
        if (block.x < 0 || block.x + BLOCK_SIZE > GAME_WIDTH)
        {
            return false;
        }
        if (block.y < 0 || block.y + BLOCK_SIZE > GAME_HEIGHT)
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

                if (is_block_in_area(block.x, block.y, row, col))
                {
                    return false;
                }
            }
        }
    }

    return true;
}

static void rotate_piece_clockwise(Piece *piece)
{
    BlocksArray *rotatedBlocks = compute_new_blocks_after_clockwise_rotation(piece);
    for (size_t i = 1; i < piece->numberOfBlocks; i++)
    {
        Point rotatedBlock = rotatedBlocks->blocks[i];
        piece->blocks[i].x = rotatedBlock.x;
        piece->blocks[i].y = rotatedBlock.y;
    }
}

static BlocksArray *compute_new_blocks_after_clockwise_rotation(Piece *piece)
{
    BlocksArray *blocksArray = (BlocksArray *)malloc(sizeof(BlocksArray));
    Point *blocks = (Point *)malloc(piece->numberOfBlocks * sizeof(Point));
    blocksArray->blocks = blocks;
    const int x_fix = piece->blocks[0].x;
    const int y_fix = piece->blocks[0].y;
    blocksArray->blocks[0].x = x_fix;
    blocksArray->blocks[0].y = y_fix;
    blocksArray->numberOfBlocks = piece->numberOfBlocks;
    for (size_t i = 1; i < piece->numberOfBlocks; i++)
    {
        const int x_new = x_fix - y_fix + piece->blocks[i].y;
        const int y_new = y_fix + x_fix - piece->blocks[i].x;
        blocksArray->blocks[i].x = x_new;
        blocksArray->blocks[i].y = y_new;
    }

    return blocksArray;
}

static void destroy_blocks_array(BlocksArray *blocksArray)
{
    free(blocksArray->blocks);
    free(blocksArray);
}