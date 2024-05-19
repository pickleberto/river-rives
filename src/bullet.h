#pragma once

#include <riv.h>
#include "level.h"
#include "score.h"

typedef struct bullet
{
    riv_rectf rect;
    bool isActive;
} Bullet;

void init_bullet(Bullet* b, float screen_x, float screen_y);
void update_bullet(Bullet* b, Level* l, Score* s);
void draw_bullet(Bullet* b);
