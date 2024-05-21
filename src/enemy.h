#pragma once

#include <riv.h>

typedef struct enemy
{
    riv_rectf rect;
    bool isActive;
    float speed;
    int enemy_type;
    int sprite_id;
    int flip_x;
    bool isDead;
    int deathFrame;
} Enemy;

void init_enemy(Enemy* e, riv_vec2f pos, int enemy_type);
void update_enemy(Enemy* e, float map_speed);
void draw_enemy(Enemy* e);
void kill_enemy(Enemy* e);

enum enemyType
{
    SLOW_TYPE = 0,
    FAST_TYPE = 1, 
};