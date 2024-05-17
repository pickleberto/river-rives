#include "game.h"
#include "const.h"

void init_game(Game* g, Player* p, Level* l)
{
    g->started = false;
    g->player = p;
    g->level = l;

    init_player(g->player);
    init_level(g->level);
}

void update_game(Game* g)
{
    update_level(g->level);
    update_player(g->player);

    if(border_collision(g->player->rect, *g->level))
    {
        kill_player(g->player);
    }
}

void draw_game(Game* g)
{
    riv_draw_text("game", RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, CENTER_X, CENTER_Y, 1, RIV_COLOR_LIGHTBLUE);
    draw_level(g->level);
    draw_player(g->player);
}