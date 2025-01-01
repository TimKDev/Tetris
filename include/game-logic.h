#pragma once

#include "game-config.h"
#include "stdbool.h"
#include "game-data.h"

void nextMove(GameData *gameData, GameConfig *config);

void move_piece_left(GameData *gameData);

void move_piece_right(GameData *gameData);

void move_piece_down(GameData *gameData);
