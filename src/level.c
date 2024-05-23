#include "level.h"

#include "fullmap.h"
#include "enemypool.h"

EnemyPool enemies;

void init_level(Level* l)
{
    l->screen_speed = BASE_MAP_SPEED; // pixels per frame
    l->map_offset = 0;
    
    l->min_y = (FULL_TILES_Y - SCREEN_TILES_Y) - 1;
    l->max_y = FULL_TILES_Y;

    init_enemyPool(&enemies);
}

void add_enemies(Level* l)
{
    for(int i = 0; i < SCREEN_TILES_X; i++)
    {
        for(int j = l->min_y; j < l->max_y; j++)
        {
            if(full_level_map[j][i] >= ENEMY_SLOW)
            {
                Enemy* e = get_enemy(&enemies);

                if(e != NULL_POINTER)
                {
                    int enemy_x = i * TILE_SIZE;
                    int enemy_y = (j - l->min_y) * TILE_SIZE + l->map_offset;
                    init_enemy(e, (riv_vec2f) {.x = enemy_x, .y = enemy_y,}, full_level_map[j][i] - ENEMY_SLOW);
                    full_level_map[j][i] = RIVER;
                }
            }
        }
    }
}

void update_level(Level* l)
{
    
    if(l->min_y <= 0) // lock map on final screen
    {
        update_enemyPool(&enemies, 0);
        return;
    }

    if(l->map_offset + l->screen_speed >= TILE_SIZE)
    {
        l->map_offset = (l->map_offset + l->screen_speed) - TILE_SIZE;
        l->min_y--;
        l->max_y--;

        if(l->min_y <= 0) // final screen
        {
            l->min_y = 0;
            l->max_y = SCREEN_TILES_Y + 1;
        }

        add_enemies(l);
    }
    else
    {
        l->map_offset = (l->map_offset + l->screen_speed);
    }

    update_enemyPool(&enemies, l->screen_speed);
}

void draw_tile(int tile_x, int tile_y, int tile, float offset)
{
    uint32_t color;
    switch (tile)
    {
    case RIVER:
        color = RIV_COLOR_BLUE;
        break;
    case OBSTACLE:
        color = RIV_COLOR_LIGHTBROWN;
        break;
    case BORDER:
        color = RIV_COLOR_DARKGREEN;
        break;
    case FUEL:
        color = RIV_COLOR_LIGHTPINK;
        break;
    case HOLE:
    default:
        color = RIV_COLOR_DARKSLATE;
        break;
    }
    riv_draw_rect_fill(tile_x * TILE_SIZE, (tile_y * TILE_SIZE) + offset - TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
}

void draw_level(Level* l)
{
    for(int i = 0; i < SCREEN_TILES_X; i++)
    {
        for(int j = l->min_y; j < l->max_y; j++)
        {
            // remember full_level_map is inverted
            draw_tile(i, j - l->min_y, full_level_map[j][i], l->map_offset);
        }
    }

    draw_enemyPool(&enemies);
}

bool screen_tile_collision(float x, float y, Level l, Score* s)
{
    int tile_x = x / SCREEN_TILES_X;
    int tile_y = ((y - l.map_offset) / SCREEN_TILES_Y) + l.min_y + 1;

    // remember full_level_map is inverted
    if(full_level_map[tile_y][tile_x] >= OBSTACLE)
    {
        if(full_level_map[tile_y][tile_x] == OBSTACLE) add_obstacle_score(s);

        full_level_map[tile_y][tile_x] = RIVER; //if destructable, then destroy it
        return true;
    }

    return full_level_map[tile_y][tile_x] >= BORDER;
}

bool tile_collision(riv_rectf object, Level l, Score* s)
{
    bool edge1 = screen_tile_collision(object.x,                   object.y,                   l,   s);
    bool edge2 = screen_tile_collision(object.x + object.width,    object.y,                   l,   s);
    bool edge3 = screen_tile_collision(object.x,                   object.y + object.height,   l,   s);
    bool edge4 = screen_tile_collision(object.x + object.width,    object.y + object.height,   l,   s);
    
    return edge1 || edge2 || edge3 || edge4;
}

bool screen_player_tile_collision(float x, float y, Level l, Score* s)
{
    int tile_x = x / SCREEN_TILES_X;
    int tile_y = ((y - l.map_offset) / SCREEN_TILES_Y) + l.min_y + 1;

    // remember full_level_map is inverted
    if(full_level_map[tile_y][tile_x] >= OBSTACLE && full_level_map[tile_y][tile_x] != FUEL)
    {
        if(full_level_map[tile_y][tile_x] == OBSTACLE) add_obstacle_score(s);

        full_level_map[tile_y][tile_x] = RIVER; //if destructable, then destroy it
        return true;
    }

    if(full_level_map[tile_y][tile_x] == FUEL)
    {
        add_fuel(s);
    }

    return full_level_map[tile_y][tile_x] >= BORDER && full_level_map[tile_y][tile_x] != FUEL;
}

bool player_tile_collision(riv_rectf object, Level l, Score* s)
{
    bool edge1 = screen_player_tile_collision(object.x,                   object.y,                   l,   s);
    bool edge2 = screen_player_tile_collision(object.x + object.width,    object.y,                   l,   s);
    bool edge3 = screen_player_tile_collision(object.x,                   object.y + object.height,   l,   s);
    bool edge4 = screen_player_tile_collision(object.x + object.width,    object.y + object.height,   l,   s);
    
    return edge1 || edge2 || edge3 || edge4;
}

bool enemies_collision(riv_rectf object, Score* s)
{
    if(collison_enemyPool(&enemies, object))
    {
        add_enemy_score(s);
        return true;
    }
    return false;
}