#include "player.h"
#include "const.h"

#define SCALE_X 1
#define SCALE_Y 1

void init_player(Player* p)
{
    p->isDead = false;
    p->pos = (riv_vec2i) { .x = CENTER_X - HALF_TILE, .y = SCREEN_HEIGHT - TILE_SIZE - HALF_TILE };
    p->sprite_id = 0;
}

void update_player(Player* p)
{

}

void draw_player(Player* p)
{
    riv_draw_sprite(p->sprite_id, GAME_SPRITESHEET, p->pos.x, p->pos.y, 1, 1, SCALE_X, SCALE_Y);
}