#pragma once

#include <gtk/gtk.h>
#include "render-logic.h"

GameContext *create_game_context(GtkWidget *gamePage, GtkWidget *gameArea, char *playerName, void (*quitCallback)(GtkWidget *widget));
void quit_game(GameContext *gameContext);
void restart_game(GameContext *context);
