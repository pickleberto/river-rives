#include "player.h"
#include "const.h"
#include "sounds.h"

#define SCALE_X 1
#define SCALE_Y 1

#define PIXELS_PER_FRAME ((float) TILE_SIZE/TARGET_FPS)
#define BASE_SPEED (2.f * PIXELS_PER_FRAME)

void init_player(Player* p)
{
    p->isDead = false;
    p->rect = (riv_rectf) { 
        .x = CENTER_X - HALF_TILE, 
        .y = SCREEN_HEIGHT - TILE_SIZE - HALF_TILE,
        .width = TILE_SIZE, .height = TILE_SIZE, 
    };
    p->sprite_id = 0;
    p->flip_x = 1;
}

void update_player(Player* p)
{
    if(p->isDead) return;

    int horizontal = 0;
    p->sprite_id = 0;
    p->flip_x = 1;
    
    if (riv->keys[RIV_GAMEPAD_LEFT].down) {
        horizontal = -1;
        p->sprite_id = 1;
        p->flip_x = -1;
    } else if (riv->keys[RIV_GAMEPAD_RIGHT].down) {
        horizontal = 1;
        p->sprite_id = 1;
    }
    
    p->rect.x += BASE_SPEED * horizontal;
}

void draw_player(Player* p)
{
    if(p->isDead)
    {
        riv_draw_rect_fill(p->rect.x, p->rect.y, TILE_SIZE, TILE_SIZE, RIV_COLOR_RED);
    }
    else
    {
        riv_draw_sprite(p->sprite_id, GAME_SPRITESHEET, p->rect.x, p->rect.y, 1, 1, SCALE_X * p->flip_x, SCALE_Y);
    }
}

void kill_player(Player* p)
{
    p->isDead = true;
    sfx_explosion();
}