#include "score.h"

#include "const.h"

void init_score(Score* s)
{
    s->score = 0;
    s->ticks = 0;
    s->obstacles_destroyed = 0;
}

void update_score(Score* s) 
{
    s->ticks++;
    s->score = (s->obstacles_destroyed * 100) - (s->ticks / 10);
    riv->outcard_len = riv_snprintf((char*)riv->outcard, RIV_SIZE_OUTCARD, 
        "JSON{\"score\":%d,\"obstacles\":%d,\"ticks\":%d}", s->score, s->obstacles_destroyed, s->ticks);
}

void draw_score(Score* s)
{
    // Draw score
    char buf[128];
    riv_snprintf(buf, sizeof(buf), "SCORE %d", s->score);
    riv_draw_text(buf, RIV_SPRITESHEET_FONT_3X5, RIV_BOTTOMLEFT, 1, SCREEN_HEIGHT - 1, 1, RIV_COLOR_WHITE);
}

void add_obstacle(Score* s)
{
    s->obstacles_destroyed++;
}