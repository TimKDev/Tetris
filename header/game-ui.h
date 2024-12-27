#ifndef GAME_UI_H
#define GAME_UI_H

#include <gtk/gtk.h>
#include "render-logic.h"

GtkWidget *create_game_page(GtkWidget *window, void (*quitCallback)(GtkWidget *widget));
void show_game_over_dialog(GameContext *context);
void update_score_display(GameContext *context, int score);
void destroy_game_page_ui_elements(GameContext *gameContext);

#endif