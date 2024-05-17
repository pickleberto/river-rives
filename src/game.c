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
    if(g->player->isDead) return;
    
    update_level(g->level);
    update_player(g->player);

    if(border_collision(g->player->rect, *g->level))
    {
        kill_player(g->player);
        
        // Quit in 3 seconds
        riv->quit_frame = riv->frame + 3*riv->target_fps;
    }
}

void draw_game(Game* g)
{
    draw_level(g->level);
    draw_player(g->player);
    
    if(g->player->isDead)
    {
        riv_draw_text("game over", RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, CENTER_X, CENTER_Y, 2, RIV_COLOR_RED);
    }
}