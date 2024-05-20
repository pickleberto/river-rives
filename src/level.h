#pragma once

#include <riv.h>
#include "const.h"
#include "score.h"

#define BASE_MAP_SPEED 1

typedef struct level
{
    float screen_speed;
    float map_offset;
    int min_y, max_y; // index range to be displayed on screen
} Level;

void init_level(Level* l);
void update_level(Level* l);
void draw_level(Level* l);
bool tile_collision(riv_rectf object, Level l, Score* s);
bool player_tile_collision(riv_rectf object, Level l, Score* s);
bool enemies_collision(riv_rectf object, Score* s);

enum tiles
{
    HOLE = -1,
    RIVER = 0,
    BORDER = 1,
    OBSTACLE = 2,
    FUEL = 3,
    ENEMY = 4,
};