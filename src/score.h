#pragma once

#include <riv.h>

typedef struct score
{
    int score;
    int ticks;
    int obstacles_destroyed;
    int fuel;
    bool completed;
} Score;

void init_score(Score* s);
void update_score(Score* s);
void draw_score(Score* s);
void add_obstacle(Score* s);
void add_fuel(Score* s);
void add_completion_bonus(Score* s);