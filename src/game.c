#include "game.h"
#include "const.h"

void init_game(Game* g, Player* p)
{
    g->started = false;
    g->player = p;

    init_player(g->player);
}

void update_game(Game* g)
{
    update_player(g->player);
}

void draw_game(Game* g)
{
    riv_draw_text("game", RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, CENTER_X, CENTER_Y, 1, RIV_COLOR_LIGHTBLUE);

    draw_player(g->player);
}