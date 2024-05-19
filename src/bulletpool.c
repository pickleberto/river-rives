#include "bulletpool.h"

#include "const.h"

void init_bulletPool(BulletPool* bp)
{
    for(int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        bp->pool[i].isActive = false;
    }
}

void update_bulletPool(BulletPool* bp, Level* l)
{
    for(int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        update_bullet(&bp->pool[i], l);
    }
}

void draw_bulletPool(BulletPool* bp)
{
    for(int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        draw_bullet(&bp->pool[i]);
    }
}

Bullet* get_bullet(BulletPool* bp)
{
    Bullet* b = NULL_POINTER;

    for(int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        if(!bp->pool[i].isActive)
        {
            b = &bp->pool[i];
            break;
        }
    }

    return b;
}