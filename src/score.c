#include "score.h"

#include "const.h"
#include "sounds.h"

#define MAX_FUEL (10 * TARGET_FPS)
#define FUEL_WIDTH (2 * TILE_SIZE)
#define FUEL_HEIGHT (HALF_TILE)

void init_score(Score* s)
{
    s->score = 0;
    s->ticks = 0;
    s->obstacles_destroyed = 0;
    s->fuel = MAX_FUEL;
    s->completed = false;
}

void update_score(Score* s) 
{
    s->ticks++;
    s->fuel--;
    int bonus = s->completed ? 1000 : 0;
    s->score = (s->obstacles_destroyed * 100) + s->fuel - (s->ticks / 10) + bonus;
    riv->outcard_len = riv_snprintf((char*)riv->outcard, RIV_SIZE_OUTCARD
        , "JSON{\"score\":%d,\"obstacles\":%d,\"ticks\":%d,\"completed\":%d}"
        , s->score, s->obstacles_destroyed, s->ticks, s->completed);
}

void draw_score(Score* s)
{
    const int text_size = 1;
    // Draw score
    char buf[128];
    riv_snprintf(buf, sizeof(buf), "SCORE %d", s->score);
    riv_draw_text(buf, RIV_SPRITESHEET_FONT_3X5, RIV_BOTTOMLEFT, 1, SCREEN_HEIGHT - 1, text_size, RIV_COLOR_WHITE);

    // Draw fuel
    const int fuel_x = SCREEN_WIDTH - FUEL_WIDTH - 10;
    const int fuel_y = SCREEN_HEIGHT - HALF_TILE - 1;
    // fuel background
    riv_draw_rect_fill(fuel_x, fuel_y, FUEL_WIDTH, FUEL_HEIGHT, RIV_COLOR_PURPLE);
    // fuel fill
    float fill = (float)s->fuel / MAX_FUEL;
    riv_draw_rect_fill(fuel_x, fuel_y, FUEL_WIDTH * fill, FUEL_HEIGHT, RIV_COLOR_PINK);
    // fuel label
    riv_draw_text(s->fuel > 0 ? "FUEL":"OUT OF GAS", RIV_SPRITESHEET_FONT_3X5, RIV_BOTTOM, fuel_x + FUEL_WIDTH / 2, fuel_y - 1, text_size, RIV_COLOR_WHITE);
}

void add_obstacle(Score* s)
{
    s->obstacles_destroyed++;
}

void add_fuel(Score* s)
{
    s->fuel = (s->fuel + 10) > MAX_FUEL ? MAX_FUEL : (s->fuel + 10);
    if(s->fuel == MAX_FUEL)
    {
        sfx_max_fuel();
    }
    else
    {
        sfx_fuel();
    }
}

void add_completion_bonus(Score* s)
{
    s->completed = true;
}