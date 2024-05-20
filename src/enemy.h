#pragma once

#include <riv.h>

typedef struct enemy
{
    riv_rectf rect;
    bool isActive;
} Enemy;

void init_enemy(Enemy* e, riv_vec2f pos);
void update_enemy(Enemy* e, float map_speed);
void draw_enemy(Enemy* e);
