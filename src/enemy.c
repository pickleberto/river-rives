#include "enemy.h"

#include "const.h"

#define ENEMY_SPEED 1

void init_enemy(Enemy* e, riv_vec2f pos)
{
    e->rect = (riv_rectf) { 
        .x = pos.x, .y = pos.y,
        .width = TILE_SIZE, .height = TILE_SIZE, 
    };
    e->isActive = true;
}

void update_enemy(Enemy* e, float map_speed)
{
    if(!e->isActive) return;
    
    e->rect.y += map_speed;
    e->rect.x += ENEMY_SPEED;

    if(e->rect.y > SCREEN_HEIGHT)
    {
        e->isActive = false;
    }
}

void draw_enemy(Enemy* e)
{
    if(!e->isActive) return;

    riv_draw_rect_fill(e->rect.x, e->rect.y, e->rect.width, e->rect.height, RIV_COLOR_LIGHTRED);
}
