#pragma once

#include <riv.h>
#include "level.h"

typedef struct bullet
{
    riv_rectf rect;
    bool isActive;
} Bullet;

void init_bullet(Bullet* b, float screen_x, float screen_y);
void update_bullet(Bullet* b, Level* l);
void draw_bullet(Bullet* b);
