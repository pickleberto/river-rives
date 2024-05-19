#pragma once

#include <riv.h>

typedef struct score
{
    int score;
    int ticks;
    int obstacles_destroyed;
} Score;

void init_score(Score* s);
void update_score(Score* s);
void draw_score(Score* s);
void add_obstacle(Score* s);