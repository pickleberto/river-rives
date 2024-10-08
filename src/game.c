#include "game.h"
#include "const.h"
#include "music.h"

Score s;

void init_game(Game* g, Player* p, Level* l)
{
    g->started = false;
    g->player = p;
    g->level = l;
    g->game_over = false;

    init_player(g->player, &s);
    init_level(g->level);
}

void update_game(Game* g)
{
    update_level(g->level);
    update_player(g->player, g->level);

    if(g->game_over) 
    {
        if(g->player->isDead)
        {
            play_game_over();
        }
        else
        {
            play_completion();
        }
        return;
    }
    
    play_music();

    if(player_tile_collision(g->player->rect, *g->level, g->player->score) 
        || enemies_collision(g->player->rect, g->player->score)
        || (g->player->score->fuel <= 0))
    {
        kill_player(g->player);
        
        // Quit in 3 seconds
        riv->quit_frame = riv->frame + 3*riv->target_fps;
        g->game_over = true;
    }
    
    if(g->level->min_y <= 0)
    {
        g->player->isFinalScreen = true;
    }

    if(g->player->rect.y <= 0)
    {
        // Quit in 3 seconds
        riv->quit_frame = riv->frame + 3*riv->target_fps;
        g->game_over = true;
    }

    if(g->game_over)
    {
        g->level->screen_speed = 0;
    }
}

void draw_game(Game* g)
{
    draw_level(g->level);
    draw_player(g->player);
    
    if(g->game_over)
    {
        if(g->player->isDead)
        {
            riv_draw_text("game over", RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, CENTER_X, CENTER_Y, 2, RIV_COLOR_RED);
        }
        else
        {
            int blink = TARGET_FPS / 5;
            uint32_t color = (riv->frame % blink < (blink/2)) ? RIV_COLOR_GOLD : RIV_COLOR_YELLOW;
            riv_draw_text("YOU WIN", RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, CENTER_X, CENTER_Y, 2, color);
        }
    }
}