#include "enemy.h"

#include "const.h"

#define ENEMY_SPEED 1
#define ENEMY_SLOW_BASE_SPRITE_ID 2
#define ENEMY_FAST_BASE_SPRITE_ID 4
#define DEATH_ANIM_TIME (1 * TARGET_FPS)

void init_enemy(Enemy* e, riv_vec2f pos, int enemy_type)
{
    e->rect = (riv_rectf) { 
        .x = pos.x, .y = pos.y,
        .width = TILE_SIZE, .height = TILE_SIZE, 
    };
    e->isActive = true;
    e->isDead = false;
    e->speed = ENEMY_SPEED;
    e->enemy_type = enemy_type;
    e->flip_x = -1;
    e->sprite_id = ENEMY_SLOW_BASE_SPRITE_ID;

    if(pos.x > SCREEN_WIDTH/2)
    {
        e->speed *= -1;
        e->flip_x = 1;
    }

    if(enemy_type == FAST_TYPE)
    {
        e->speed *= 2;
        e->sprite_id = ENEMY_FAST_BASE_SPRITE_ID;
    }
}

void update_enemy(Enemy* e, float map_speed)
{
    if(!e->isActive) return;

    if(e->isDead)
    {
        e->rect.y += map_speed;
        e->sprite_id = EXPLOSION_SPRITE_ID  + (riv->frame / ANIM_RATE) % ANIM_SPRITES;

        if(e->deathFrame + DEATH_ANIM_TIME <= riv->frame)
        {
            e->isActive = false;
        }
    }
    else
    {
        e->rect.y += map_speed;
        e->rect.x += e->speed;
        int base_id = e->enemy_type == FAST_TYPE ? ENEMY_FAST_BASE_SPRITE_ID : ENEMY_SLOW_BASE_SPRITE_ID;
        e->sprite_id = base_id + (riv->frame / ANIM_RATE) % ANIM_SPRITES;

        if(e->rect.y > SCREEN_HEIGHT || e->rect.x > SCREEN_WIDTH || e->rect.x < 0)
        {
            e->isActive = false;
        }
    }
}

void draw_enemy(Enemy* e)
{
    if(!e->isActive) return;

    riv_draw_sprite(e->sprite_id, GAME_SPRITESHEET, e->rect.x, e->rect.y, 1, 1, SPRITE_SCALE_X * e->flip_x, SPRITE_SCALE_Y);
}

void kill_enemy(Enemy* e)
{
    e->deathFrame = riv->frame;
    e->isDead = true;
}
