#ifndef BULLET_POOL_H
#define BULLET_POOL_H

#include <riv.h>
#include "bullet.h"

#define BULLET_POOL_SIZE 20

typedef struct bulletPool
{
    Bullet pool[BULLET_POOL_SIZE];
} BulletPool;

void init_bulletPool(BulletPool* bp);
void update_bulletPool(BulletPool* bp, Level* l, Score* s);
void draw_bulletPool(BulletPool* bp);
Bullet* get_bullet(BulletPool* bp);

#endif