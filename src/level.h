#pragma once

#include "const.h"

#define SCREEN_TILES_X (SCREEN_WIDTH / TILE_SIZE)
#define SCREEN_TILES_Y (SCREEN_HEIGHT / TILE_SIZE)

typedef struct level
{
    int screen_tiles[SCREEN_TILES_X][SCREEN_TILES_Y];
} Level;

void init_level(Level* l);
void update_level(Level* l);
void draw_level(Level* l);

enum tiles
{
    RIVER = 0,
    BORDER = 1,
};