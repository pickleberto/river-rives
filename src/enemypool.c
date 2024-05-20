#include "enemypool.h"

#include "const.h"

void init_enemyPool(EnemyPool* ep)
{
    for(int i = 0; i < ENEMY_POOL_SIZE; i++)
    {
        ep->pool[i].isActive = false;
    }
}

void update_enemyPool(EnemyPool* ep, float map_speed)
{
    for(int i = 0; i < ENEMY_POOL_SIZE; i++)
    {
        update_enemy(&ep->pool[i], map_speed);
    }
}

void draw_enemyPool(EnemyPool* ep)
{
    for(int i = 0; i < ENEMY_POOL_SIZE; i++)
    {
        draw_enemy(&ep->pool[i]);
    }
}

Enemy* get_enemy(EnemyPool* ep)
{
    Enemy* e = NULL_POINTER;

    for(int i = 0; i < ENEMY_POOL_SIZE; i++)
    {
        if(!ep->pool[i].isActive)
        {
            e = &ep->pool[i];
            break;
        }
    }

    return e;
}