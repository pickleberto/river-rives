#include "player.h"
#include "const.h"

void init_player(Player* p)
{
    p->isDead = false;
    p->pos = (riv_vec2i) { .x = CENTER_X - HALF_TILE, .y = SCREEN_HEIGHT - TILE_SIZE - HALF_TILE };
}

void update_player(Player* p)
{

}

void draw_player(Player* p)
{
    riv_draw_rect_fill(p->pos.x, p->pos.y, TILE_SIZE, TILE_SIZE, RIV_COLOR_YELLOW);
}