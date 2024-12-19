#include <stdlib.h>
#include "../header/gameLogic.h"

static GameData *gameData;
static GameConfig *config;

GameData *initialize()
{
    srand(time(NULL));
    config = load_config_from_file("config/game.json");
    gameData = (GameData *)calloc(1, sizeof(GameData));
}

void clean_up()
{
    // Guard Clause für den Fall, dass initialize noch nicht aufgerufen wurde.
    destroy_game_config(config);
    destory_game_data();
}

GameData *nextMove()
{
    // Guard Clause für den Fall, dass initialize noch nicht aufgerufen wurde.
}

Piece CreateRandomPiece()
{
    int randomColorPosition = rand() % config->colorsCount;
    ColorConfig randomColorConfig = config->colors[randomColorPosition];
    int randomPiecePosition = rand() % config->piecesCount;
    Point *randomPieceConfig = config->pieces[randomPiecePosition];
    int numberOfBlocksInRandomPiece = config->pieceSizes[randomPiecePosition];
}

void destory_game_data()
{
    // Hier kommt die free Logik rein für die GameData
}