#ifndef GAME_UI_H
#define GAME_UI_H

#include <gtk/gtk.h>
#include "render-logic.h"

GtkWidget *create_game_page(GtkWidget *window, char *playerName, void (*quitCallback)(GtkWidget *widget));
void update_score_display(GameContext *context);
void destroy_game_page_ui_elements(GameContext *gameContext);

#endif