#include "bullet.h"

#include "sounds.h"

#define BULLET_WIDTH 2
#define BULLET_HEIGHT 4
#define BULLET_SPEED 5

void init_bullet(Bullet* b, float screen_x, float screen_y)
{
    b->rect.x = screen_x;
    b->rect.y = screen_y;
    b->rect.width = BULLET_WIDTH;
    b->rect.height = BULLET_HEIGHT;
    b->isActive = true;
}

void update_bullet(Bullet* b, Level* l, Score* s)
{
    if(!b->isActive) return;

    b->rect.y -= BULLET_SPEED;
    if(b->rect.y < -BULLET_HEIGHT)
    {
        b->isActive = false;
    }

    if(tile_collision(b->rect, *l, s) || enemies_collision(b->rect, s))
    {
        b->isActive = false;
        sfx_explosion();
    }
}

void draw_bullet(Bullet* b)
{
    if(!b->isActive) return;

    riv_draw_rect_fill(b->rect.x, b->rect.y, b->rect.width, b->rect.height, RIV_COLOR_YELLOW);
}