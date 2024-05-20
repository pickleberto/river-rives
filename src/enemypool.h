#pragma once

#include <riv.h>
#include "enemy.h"

#define ENEMY_POOL_SIZE 10

typedef struct enemyPool
{
    Enemy pool[ENEMY_POOL_SIZE];
} EnemyPool;

void init_enemyPool(EnemyPool* ep);
void update_enemyPool(EnemyPool* ep, float map_speed);
void draw_enemyPool(EnemyPool* ep);
Enemy* get_enemy(EnemyPool* ep);