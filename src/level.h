#pragma once

#include <riv.h>
#include "const.h"
#include "score.h"

typedef struct level
{
    int screen_speed;
    int map_offset;
    int min_y, max_y; // index range to be displayed on screen
} Level;

void init_level(Level* l);
void update_level(Level* l);
void draw_level(Level* l);
bool tile_collision(riv_rectf object, Level l, Score* s);


enum tiles
{
    HOLE = -1,
    RIVER = 0,
    BORDER = 1,
    OBSTACLE = 2,
};