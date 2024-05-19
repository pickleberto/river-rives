#include "player.h"
#include "const.h"
#include "sounds.h"
#include "bulletpool.h"

#define SCALE_X 1
#define SCALE_Y 1

#define PIXELS_PER_FRAME ((float) TILE_SIZE/TARGET_FPS)
#define BASE_SPEED (2.f * PIXELS_PER_FRAME)
#define SHOOT_COOLDOWN_FRAMES (uint64_t) (0.25f * TARGET_FPS)

BulletPool bullets;

void init_player(Player* p)
{
    p->isDead = false;
    p->rect = (riv_rectf) { 
        .x = CENTER_X - HALF_TILE, 
        .y = SCREEN_HEIGHT - TILE_SIZE - HALF_TILE,
        .width = TILE_SIZE, .height = TILE_SIZE, 
    };
    p->sprite_id = 0;
    p->flip_x = 1;
    p->isFinalScreen = false;

    init_bulletPool(&bullets);
}

void update_vertical(Player* p, Level* l)
{
    if(!p->isFinalScreen) return;
    
    p->rect.y -= l->screen_speed;
}

void update_horizontal(Player* p)
{
    int horizontal = 0;
    p->sprite_id = 0;
    p->flip_x = 1;
    
    if (riv->keys[RIV_GAMEPAD_LEFT].down) {
        horizontal = -1;
        p->sprite_id = 1;
        p->flip_x = -1;
    } else if (riv->keys[RIV_GAMEPAD_RIGHT].down) {
        horizontal = 1;
        p->sprite_id = 1;
    }
    p->rect.x += BASE_SPEED * horizontal;

    if(horizontal != 0)
    {
        sfx_move();
    }
}

uint64_t last_shoot = 0ULL;
void update_fire(Player* p, Level* l)
{
    if(riv->keys[RIV_GAMEPAD_A1].down && (last_shoot + SHOOT_COOLDOWN_FRAMES <= riv->frame))
    {
        Bullet* b = get_bullet(&bullets);
        if(b != NULL_POINTER)
        {
            init_bullet(b, p->rect.x + (p->rect.width / 2), p->rect.y);
            sfx_shoot();
            last_shoot = riv->frame;
        }
    }

    update_bulletPool(&bullets, l);
}

void update_player(Player* p, Level* l)
{
    if(p->isDead) return;
    
    update_horizontal(p);
    update_fire(p, l);
    update_vertical(p, l);   
}

void draw_player(Player* p)
{
    if(p->isDead)
    {
        riv_draw_rect_fill(p->rect.x, p->rect.y, TILE_SIZE, TILE_SIZE, RIV_COLOR_RED);
    }
    else
    {
        riv_draw_sprite(p->sprite_id, GAME_SPRITESHEET, p->rect.x, p->rect.y, 1, 1, SCALE_X * p->flip_x, SCALE_Y);
    }

    draw_bulletPool(&bullets);
}

void kill_player(Player* p)
{
    p->isDead = true;
    sfx_explosion();
}