#include "level.h"

#include <riv.h>

// invert here for better visualization
int full_level_map[SCREEN_TILES_Y][SCREEN_TILES_X] = 
{
    {1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
    {1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1},
    {1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1},
    {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1},
    {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1},
    {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1},
    {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1},
    {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1},
    {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1},
    {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1},
    {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1},
    {1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1},
    {1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1},
    {1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
};



void init_level(Level* l)
{
    for(int i = 0; i < SCREEN_TILES_X; i++)
    {
        for(int j = 0; j < SCREEN_TILES_Y; j++)
        {
            // remember full_level_map is inverted
            l->screen_tiles[i][j] = full_level_map[j][i];
        }
    }
    // l->screen_tiles = full_level_map;
}
void update_level(Level* l)
{

}

void draw_tile(int tile_x, int tile_y, int tile)
{
    uint32_t color;
    switch (tile)
    {
    case RIVER:
        color = RIV_COLOR_BLUE;
        break;
    case BORDER:
    default:
        color = RIV_COLOR_DARKGREEN;
        break;
    }
    riv_draw_rect_fill(tile_x * TILE_SIZE, tile_y * TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
}

void draw_level(Level* l)
{
    for(int i = 0; i < SCREEN_TILES_X; i++)
    {
        for(int j = 0; j < SCREEN_TILES_Y; j++)
        {
            draw_tile(i, j, l->screen_tiles[i][j]);
        }
    }
}


