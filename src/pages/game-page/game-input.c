#include "game-input.h"
#include "game-context.h"
#include "game-logic.h"
#include "render-logic.h"

gboolean handle_key_press(GtkEventControllerKey *controller,
                          guint keyval,
                          guint keycode,
                          GdkModifierType state,
                          gpointer user_data)
{
    GameContext *context = (GameContext *)user_data;

    if (context->is_paused)
    {
        return FALSE;
    }

    switch (keyval)
    {
    case GDK_KEY_Left:
        move_piece_left(context->game_data);
        break;

    case GDK_KEY_Right:
        move_piece_right(context->game_data);
        break;

    case GDK_KEY_Up:
        printf("Up arrow pressed\n");
        break;

    case GDK_KEY_Down:
        move_piece_down(context->game_data);
        break;

    default:
        return FALSE; // Event not handled
    }

    render_game_data(context);
    return TRUE; // Event handled
}
