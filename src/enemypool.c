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

bool enemy_collision(riv_rectf object, riv_rectf enemy)
{
    float x1 = object.x + object.width;
    float y1 = object.y + object.height;
    float ex1 = enemy.x + enemy.width;
    float ey1 = enemy.y + enemy.height;

    return 
        ((object.x >= enemy.x && object.x <= ex1) ||  (x1 >= enemy.x && x1 <= ex1))
    &&  ((object.y >= enemy.y && object.y <= ey1) ||  (y1 >= enemy.y && y1 <= ey1));
}

bool collison_enemyPool(EnemyPool* ep, riv_rectf object)
{
    for (int i = 0; i < ENEMY_POOL_SIZE; i++)
    {
        if(ep->pool[i].isActive && enemy_collision(object, ep->pool[i].rect))
        {
            ep->pool[i].isActive = false;
            return true;
        }
    }
    return false;
}