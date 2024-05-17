#include "game.h"
#include "const.h"

void init_game(Game* g)
{
    g->started = false;
}

void update_game()
{

}

void draw_game()
{
    riv_draw_text("game", RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, CENTER_X, CENTER_Y, 1, RIV_COLOR_LIGHTBLUE);
}