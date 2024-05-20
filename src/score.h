#ifndef SCORE_H
#define SCORE_H

#include <riv.h>

typedef struct score
{
    int score;
    int ticks;
    int obstacles_destroyed;
    int fuel;
} Score;

void init_score(Score* s);
void update_score(Score* s);
void draw_score(Score* s);
void add_obstacle(Score* s);
void add_fuel(Score* s);

#endif