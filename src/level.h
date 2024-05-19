#pragma once

#include <riv.h>
#include "const.h"

#define SCREEN_TILES_X (SCREEN_WIDTH / TILE_SIZE)
#define SCREEN_TILES_Y (SCREEN_HEIGHT / TILE_SIZE)

typedef struct level
{
    int screen_speed;
    int map_offset;
    int min_y, max_y; // index range to be displayed on screen
} Level;

void init_level(Level* l);
void update_level(Level* l);
void draw_level(Level* l);
bool border_collision(riv_rectf object, Level l);


enum tiles
{
    RIVER = 0,
    BORDER = 1,
};