#include "enemy.h"

#include "const.h"

#define ENEMY_SPEED 1
#define ENEMY_BASE_SPRITE_ID 2
#define ANIM_SPRITES 2
#define ANIM_RATE (TARGET_FPS / 10)

void init_enemy(Enemy* e, riv_vec2f pos, int enemy_type)
{
    e->rect = (riv_rectf) { 
        .x = pos.x, .y = pos.y,
        .width = TILE_SIZE, .height = TILE_SIZE, 
    };
    e->isActive = true;
    e->speed = ENEMY_SPEED;
    e->enemy_type = enemy_type;
    e->flip_x = -1;
    e->sprite_id = ENEMY_BASE_SPRITE_ID;

    if(pos.x > SCREEN_WIDTH/2)
    {
        e->speed *= -1;
        e->flip_x = 1;
    }

    if(enemy_type == FAST_TYPE)
    {
        e->speed *= 2;
    }
}

void update_enemy(Enemy* e, float map_speed)
{
    if(!e->isActive) return;

    e->rect.y += map_speed;
    e->rect.x += e->speed;
    e->sprite_id = ENEMY_BASE_SPRITE_ID + (riv->frame / ANIM_RATE) % ANIM_SPRITES;

    if(e->rect.y > SCREEN_HEIGHT || e->rect.x > SCREEN_WIDTH || e->rect.x < 0)
    {
        e->isActive = false;
    }
}

void draw_enemy(Enemy* e)
{
    if(!e->isActive) return;

    //riv_draw_rect_fill(e->rect.x, e->rect.y, e->rect.width, e->rect.height, RIV_COLOR_LIGHTRED);
    riv_draw_sprite(e->sprite_id, GAME_SPRITESHEET, e->rect.x, e->rect.y, 1, 1, SPRITE_SCALE_X * e->flip_x, SPRITE_SCALE_Y);
}
