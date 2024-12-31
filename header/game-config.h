#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <json-c/json.h>

typedef struct
{
    int x;
    int y;
} Point;

typedef enum
{
    Empty,
    Blue,
    Red,
    Green,
    Yellow,
    Purple
} GridValue;

typedef struct
{
    GridValue key;
    const char *className;
    int scoreValue;
} ColorConfig;

typedef struct
{
    ColorConfig *colors;
    int colorsCount;
    Point **pieces;
    int *pieceSizes;
    int piecesCount;
    int velocityDown;
    int velocityMovement;
} GameConfig;

// Function to load game configuration from JSON file
GameConfig *load_config_from_file(const char *filename);

// Function to clean up game configuration
void destroy_game_config(GameConfig *config);

#endif