#include "headers.h"




#define BULLET_WIDTH 2
#define BULLET_HEIGHT 4
#define BULLET_SPEED 5

void init_bullet(Bullet* b, float screen_x, float screen_y)
{
    b->rect.x = screen_x;
    b->rect.y = screen_y;
    b->rect.width = BULLET_WIDTH;
    b->rect.height = BULLET_HEIGHT;
    b->isActive = true;
}

void update_bullet(Bullet* b, Level* l, Score* s)
{
    if(!b->isActive) return;

    b->rect.y -= BULLET_SPEED;
    if(b->rect.y < -BULLET_HEIGHT)
    {
        b->isActive = false;
    }

    if(tile_collision(b->rect, *l, s) || enemies_collision(b->rect, s))
    {
        b->isActive = false;
        sfx_explosion();
    }
}

void draw_bullet(Bullet* b)
{
    if(!b->isActive) return;

    riv_draw_rect_fill(b->rect.x, b->rect.y, b->rect.width, b->rect.height, RIV_COLOR_YELLOW);
}



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



#define ENEMY_SPEED 1
#define ENEMY_SLOW_BASE_SPRITE_ID 2
#define ENEMY_FAST_BASE_SPRITE_ID 4
#define DEATH_ANIM_TIME (1 * TARGET_FPS)

void init_enemy(Enemy* e, riv_vec2f pos, int enemy_type)
{
    e->rect = (riv_rectf) { 
        .x = pos.x, .y = pos.y,
        .width = TILE_SIZE, .height = TILE_SIZE, 
    };
    e->isActive = true;
    e->isDead = false;
    e->speed = ENEMY_SPEED;
    e->enemy_type = enemy_type;
    e->flip_x = -1;
    e->sprite_id = ENEMY_SLOW_BASE_SPRITE_ID;

    if(pos.x > SCREEN_WIDTH/2)
    {
        e->speed *= -1;
        e->flip_x = 1;
    }

    if(enemy_type == FAST_TYPE)
    {
        e->speed *= 2;
        e->sprite_id = ENEMY_FAST_BASE_SPRITE_ID;
    }
}

void update_enemy(Enemy* e, float map_speed)
{
    if(!e->isActive) return;

    if(e->isDead)
    {
        e->rect.y += map_speed;
        e->sprite_id = EXPLOSION_SPRITE_ID  + (riv->frame / ANIM_RATE) % ANIM_SPRITES;

        if(e->deathFrame + DEATH_ANIM_TIME <= riv->frame)
        {
            e->isActive = false;
        }
    }
    else
    {
        e->rect.y += map_speed;
        e->rect.x += e->speed;
        int base_id = e->enemy_type == FAST_TYPE ? ENEMY_FAST_BASE_SPRITE_ID : ENEMY_SLOW_BASE_SPRITE_ID;
        e->sprite_id = base_id + (riv->frame / ANIM_RATE) % ANIM_SPRITES;

        if(e->rect.y > SCREEN_HEIGHT || e->rect.x > SCREEN_WIDTH || e->rect.x < 0)
        {
            e->isActive = false;
        }
    }
}

void draw_enemy(Enemy* e)
{
    if(!e->isActive) return;

    riv_draw_sprite(e->sprite_id, GAME_SPRITESHEET, e->rect.x, e->rect.y, 1, 1, SPRITE_SCALE_X * e->flip_x, SPRITE_SCALE_Y);
}

void kill_enemy(Enemy* e)
{
    e->deathFrame = riv->frame;
    e->isDead = true;
}





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
    }    
}




#define TILES_PER_SECOND ((float) TILE_SIZE/TARGET_FPS)
#define BASE_SPEED (2.f * TILES_PER_SECOND)
#define SHOOT_COOLDOWN_FRAMES (uint64_t) (0.25f * TARGET_FPS)

#define ACCELERATION 2.5f

BulletPool bullets;

void init_player(Player* p, Score* s)
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
    p->score = s;

    init_bulletPool(&bullets);
    init_score(p->score);
}

void update_vertical(Player* p, Level* l)
{
    float y = BASE_MAP_SPEED;
    
    if(riv->keys[RIV_GAMEPAD_UP].down)
    {
        y *= ACCELERATION;
    }
    else if(riv->keys[RIV_GAMEPAD_DOWN].down)
    {
        y /= ACCELERATION;
    }

    l->screen_speed = y;

    if(p->isFinalScreen) 
    {
        p->rect.y -= l->screen_speed;
    }

    if(!p->score->completed && p->rect.y <= 0)
    {
        add_completion_bonus(p->score);
    }
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

    update_bulletPool(&bullets, l, p->score);
}

void update_player(Player* p, Level* l)
{
    if(p->isDead)
    {
        p->sprite_id = EXPLOSION_SPRITE_ID  + (riv->frame / ANIM_RATE) % ANIM_SPRITES;
    }
    else
    {
        update_horizontal(p);
        update_fire(p, l);
        update_vertical(p, l);
        update_score(p->score);
    }
}

void draw_player(Player* p)
{
    riv_draw_sprite(p->sprite_id, GAME_SPRITESHEET, p->rect.x, p->rect.y, 1, 1, SPRITE_SCALE_X * p->flip_x, SPRITE_SCALE_Y);
    draw_bulletPool(&bullets);
    draw_score(p->score);
}

void kill_player(Player* p)
{
    p->isDead = true;
    sfx_explosion();
}



void init_enemyPool(EnemyPool* ep)
{
    for(int i = 0; i < ENEMY_POOL_SIZE; i++)
    {
        ep->pool[i].isActive = false;
        ep->pool[i].isDead = false;
    }
}

void update_enemyPool(EnemyPool* ep, float map_speed)
{
    for(int i = 0; i < ENEMY_POOL_SIZE; i++)
    {
        update_enemy(&ep->pool[i], map_speed);
    }
}

void draw_enemyPool(EnemyPool* ep)
{
    for(int i = 0; i < ENEMY_POOL_SIZE; i++)
    {
        draw_enemy(&ep->pool[i]);
    }
}

Enemy* get_enemy(EnemyPool* ep)
{
    Enemy* e = NULL_POINTER;

    for(int i = 0; i < ENEMY_POOL_SIZE; i++)
    {
        if(!ep->pool[i].isActive)
        {
            e = &ep->pool[i];
            break;
        }
    }

    return e;
}

bool enemy_collision(riv_rectf object, riv_rectf enemy)
{
    float x1 = object.x + object.width;
    float y1 = object.y + object.height;
    float ex1 = enemy.x + enemy.width;
    float ey1 = enemy.y + enemy.height;

    return 
        ((object.x >= enemy.x && object.x <= ex1) ||  (x1 >= enemy.x && x1 <= ex1))
    &&  ((object.y >= enemy.y && object.y <= ey1) ||  (y1 >= enemy.y && y1 <= ey1));
}

bool collison_enemyPool(EnemyPool* ep, riv_rectf object)
{
    for (int i = 0; i < ENEMY_POOL_SIZE; i++)
    {
        if(ep->pool[i].isActive && !ep->pool[i].isDead && enemy_collision(object, ep->pool[i].rect))
        {
            kill_enemy(&ep->pool[i]);
            return true;
        }
    }
    return false;
}



#define MUSIC_SPEED 10
#define M_SUSTAIN .5f
#define M_ATK_REL .15f

#define COMPLETED_MUSIC_SPEED 12
#define GAME_OVER_MUSIC_SPEED 30

riv_note_freq  bass_notes[] = {
  RIV_NOTE_A4, 0, RIV_NOTE_A4, RIV_NOTE_A4, RIV_NOTE_A4, RIV_NOTE_A4, 0, RIV_NOTE_A4, RIV_NOTE_A4, RIV_NOTE_A4, RIV_NOTE_A4, 0, RIV_NOTE_A4, RIV_NOTE_A4, RIV_NOTE_C5, RIV_NOTE_A4,
  RIV_NOTE_C5, 0, RIV_NOTE_C5, RIV_NOTE_C5, RIV_NOTE_C5, RIV_NOTE_C5, 0, RIV_NOTE_C5, RIV_NOTE_C5, RIV_NOTE_C5, RIV_NOTE_C5, 0, RIV_NOTE_C5, RIV_NOTE_C5, RIV_NOTE_D5, RIV_NOTE_C5,
  RIV_NOTE_D5, 0, RIV_NOTE_D5, RIV_NOTE_D5, RIV_NOTE_D5, RIV_NOTE_D5, 0, RIV_NOTE_D5, RIV_NOTE_D5, RIV_NOTE_D5, RIV_NOTE_D5, 0, RIV_NOTE_D5, RIV_NOTE_D5, RIV_NOTE_G4, RIV_NOTE_D5,
  RIV_NOTE_G4, 0, RIV_NOTE_G4, RIV_NOTE_G4, RIV_NOTE_G4, RIV_NOTE_G4, 0, RIV_NOTE_G4, RIV_NOTE_G4, RIV_NOTE_G4, RIV_NOTE_G4, 0, RIV_NOTE_G4, RIV_NOTE_G4, RIV_NOTE_A4, RIV_NOTE_G4,
};

riv_note_freq  rhythm_notes[] = {
  RIV_NOTE_C4, RIV_NOTE_Eb4, RIV_NOTE_G4, RIV_NOTE_Bb4, RIV_NOTE_C4, RIV_NOTE_Eb4, RIV_NOTE_G4, RIV_NOTE_Bb4, RIV_NOTE_C4, RIV_NOTE_Eb4, RIV_NOTE_G4, RIV_NOTE_Bb4, RIV_NOTE_C4, RIV_NOTE_Eb4, RIV_NOTE_G4, RIV_NOTE_Bb4,
  RIV_NOTE_F4, RIV_NOTE_Ab4, RIV_NOTE_C5, RIV_NOTE_D5, RIV_NOTE_F4, RIV_NOTE_Ab4, RIV_NOTE_C5, RIV_NOTE_D5, RIV_NOTE_F4, RIV_NOTE_Ab4, RIV_NOTE_C5, RIV_NOTE_D5, RIV_NOTE_F4, RIV_NOTE_Ab4, RIV_NOTE_C5, RIV_NOTE_D5,
  RIV_NOTE_Eb4, RIV_NOTE_G4, RIV_NOTE_Bb4, RIV_NOTE_C5, RIV_NOTE_Eb4, RIV_NOTE_G4, RIV_NOTE_Bb4, RIV_NOTE_C5, RIV_NOTE_Eb4, RIV_NOTE_G4, RIV_NOTE_Bb4, RIV_NOTE_C5, RIV_NOTE_Eb4, RIV_NOTE_G4, RIV_NOTE_Bb4, RIV_NOTE_C5,
  RIV_NOTE_G4, RIV_NOTE_B4, RIV_NOTE_D5, RIV_NOTE_F5, RIV_NOTE_G4, RIV_NOTE_B4, RIV_NOTE_D5, RIV_NOTE_F5, RIV_NOTE_G4, RIV_NOTE_B4, RIV_NOTE_D5, RIV_NOTE_F5, RIV_NOTE_G4, RIV_NOTE_B4, RIV_NOTE_D5, RIV_NOTE_F5,
};

riv_note_freq lead_notes[] = {
RIV_NOTE_A4, RIV_NOTE_C5, RIV_NOTE_A4, RIV_NOTE_G4, RIV_NOTE_F4, RIV_NOTE_G4, RIV_NOTE_E4, RIV_NOTE_E4, RIV_NOTE_Gb4, RIV_NOTE_G4, RIV_NOTE_B4, RIV_NOTE_A4, RIV_NOTE_G4, RIV_NOTE_Gb4, RIV_NOTE_A4, RIV_NOTE_Gb4,
RIV_NOTE_A4, RIV_NOTE_A4, RIV_NOTE_E4, RIV_NOTE_A4, RIV_NOTE_E5, RIV_NOTE_D5, RIV_NOTE_C5, RIV_NOTE_B4, RIV_NOTE_B4, RIV_NOTE_E4, RIV_NOTE_A4, RIV_NOTE_B4, RIV_NOTE_C5, RIV_NOTE_B4, RIV_NOTE_A4, RIV_NOTE_G4,
RIV_NOTE_A4, RIV_NOTE_C5, RIV_NOTE_A4, RIV_NOTE_G4, RIV_NOTE_F4, RIV_NOTE_G4, RIV_NOTE_E4, RIV_NOTE_E4, RIV_NOTE_Gb4, RIV_NOTE_G4, RIV_NOTE_B4, RIV_NOTE_A4, RIV_NOTE_G4, RIV_NOTE_Gb4, RIV_NOTE_A4, RIV_NOTE_Gb4,
RIV_NOTE_G4, RIV_NOTE_G4, RIV_NOTE_E4, RIV_NOTE_D4, RIV_NOTE_C4, RIV_NOTE_E4, RIV_NOTE_A4, RIV_NOTE_A4, RIV_NOTE_G4, RIV_NOTE_E4, RIV_NOTE_E4, RIV_NOTE_D4, RIV_NOTE_C4, RIV_NOTE_D4, RIV_NOTE_C4, RIV_NOTE_C4,
};

riv_note_freq  completed_notes[] = {
  RIV_NOTE_A4, RIV_NOTE_A4, RIV_NOTE_Bb4, 0, RIV_NOTE_A4, RIV_NOTE_C5, 0, RIV_NOTE_G4, 0, 0, RIV_NOTE_A4, RIV_NOTE_C5, RIV_NOTE_A4, RIV_NOTE_G4, RIV_NOTE_F4, 0,
};

riv_note_freq  game_over_notes[] = {
  RIV_NOTE_C3, RIV_NOTE_B2, RIV_NOTE_G2, RIV_NOTE_E2, 
};

float durations[] ={
    1.f, 1.f, 1.f, 2.f, 
};


uint64_t next_frame = 0ULL;
int notes_index = 0;
bool play_music_notes(riv_note_freq notes[], int index, riv_waveform_type wave_type)
{
    if(riv->frame >= next_frame)
    {
        riv_waveform_desc current_note = {
            .type = wave_type,
            .attack = M_ATK_REL, .release = M_ATK_REL, .sustain_level = M_SUSTAIN,
            .duty_cycle = 0.75, .amplitude = 0.2f, 
            .start_frequency = notes[index], 
        };
    
        riv_waveform(&current_note);
        return true;
    }
    return false;
}

bool play_bass_notes(riv_note_freq notes[], int index)
{
    if(riv->frame >= next_frame)
    {
        riv_waveform_desc current_note = {
            .type = RIV_WAVEFORM_TRIANGLE,
            .attack = M_ATK_REL, .release = M_ATK_REL, .sustain_level = M_SUSTAIN,
            .duty_cycle = 0.75, .amplitude = 0.15f, 
            .start_frequency = notes[index],
        };
    
        riv_waveform(&current_note);
        return true;
    }
    return false;
}

int note_count = 0;
void play_music()
{
    if(play_bass_notes(bass_notes, notes_index))
    {
        if((note_count / N_ELEMS(rhythm_notes)) % 3 == 2)
        {
            play_music_notes(lead_notes, notes_index, RIV_WAVEFORM_SAWTOOTH);
        }else if((note_count / N_ELEMS(rhythm_notes)) % 3 == 1)
        {
            play_music_notes(rhythm_notes, notes_index, RIV_WAVEFORM_SINE);
        }
        notes_index = (notes_index + 1) % N_ELEMS(rhythm_notes);
        next_frame = riv->frame + MUSIC_SPEED;
        note_count ++;
    }
}

int completed_index = 0;
void play_completion()
{
    if(completed_index < N_ELEMS(completed_notes) && play_music_notes(completed_notes, completed_index, RIV_WAVEFORM_PULSE) )
    {
        next_frame = riv->frame + COMPLETED_MUSIC_SPEED;
        completed_index ++;
    }
}

int game_over_notes_index = 0;
void play_game_over()
{
    if(riv->frame >= next_frame && game_over_notes_index < N_ELEMS(game_over_notes))
    {
        int d_index = notes_index % 16;
        riv_waveform_desc current_note = {
            .type = RIV_WAVEFORM_SQUARE,
            .attack = .5f, .release = .5f, .sustain_level = durations[game_over_notes_index],
            .duty_cycle = 0.25, .amplitude = 0.1f, 
            .start_frequency = game_over_notes[game_over_notes_index], 
        };
    
        riv_waveform(&current_note);
        next_frame = riv->frame + GAME_OVER_MUSIC_SPEED * durations[game_over_notes_index++];
    }
}



void init_bulletPool(BulletPool* bp)
{
    for(int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        bp->pool[i].isActive = false;
    }
}

void update_bulletPool(BulletPool* bp, Level* l, Score* s)
{
    for(int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        update_bullet(&bp->pool[i], l, s);
    }
}

void draw_bulletPool(BulletPool* bp)
{
    for(int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        draw_bullet(&bp->pool[i]);
    }
}

Bullet* get_bullet(BulletPool* bp)
{
    Bullet* b = NULL_POINTER;

    for(int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        if(!bp->pool[i].isActive)
        {
            b = &bp->pool[i];
            break;
        }
    }

    return b;
}




#define MAX_FUEL (10 * TARGET_FPS)
#define FUEL_WIDTH (2 * TILE_SIZE)
#define FUEL_HEIGHT (HALF_TILE)

#define OBSTACLE_PTS 100
#define ENEMY_PTS 250
#define COMPLETION_PTS (ENEMY_PTS * 10)
#define TICK_FACTOR 10

void init_score(Score* s)
{
    s->score = 0;
    s->ticks = 0;
    s->obstacles_destroyed = 0;
    s->fuel = MAX_FUEL;
    s->completed = false;
    s->enemies = 0;
}

void update_score(Score* s) 
{
    int bonus = COMPLETION_PTS;
    if(!s->completed)
    {
        bonus = 0;
        s->ticks++;
        s->fuel--;
    }
    
    s->score = (s->obstacles_destroyed * OBSTACLE_PTS) 
        + s->fuel 
        + (s->enemies * ENEMY_PTS) 
        - (s->ticks / TICK_FACTOR) 
        + bonus;
    
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

void add_obstacle_score(Score* s)
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

void add_enemy_score(Score* s)
{
    s->enemies++;
}




#define WAVEFORM_FRAMES (uint64_t)(.5f * TARGET_FPS)

riv_waveform_desc explosion = {
    .type = RIV_WAVEFORM_NOISE,
    .attack = 0.025f, .decay = 0.1f, .sustain = 0.5f, .release = 0.025f,
    .start_frequency = RIV_NOTE_A2, .end_frequency = RIV_NOTE_A0,
    .amplitude = 1.f, .sustain_level = 0.5f, .duty_cycle = 0.5f, .pan = 0,
};

riv_waveform_desc move = {
    .type = RIV_WAVEFORM_NOISE,
    .attack = 0.025f, .decay = 0.1f, .sustain = 0.5f, .release = 0.025f,
    .start_frequency = RIV_NOTE_G1, .end_frequency = RIV_NOTE_G1,
    .amplitude = 0.5f, .sustain_level = 0.5f, .duty_cycle = 0.5f, .pan = 0,
};

riv_waveform_desc shoot = {
    .type = RIV_WAVEFORM_PULSE,
    .attack = 0.05f, .decay = 0.05f, .sustain = 0.15f, .release = 0.075f,
    .start_frequency = RIV_NOTE_A5, .end_frequency = RIV_NOTE_A3,
    .amplitude = .75f, .sustain_level = 0.25f, .duty_cycle = 0.65f, .pan = 0,
};

riv_waveform_desc fuel = {
    .type = RIV_WAVEFORM_TRIANGLE,
    .attack = 0.025f, .decay = 0.075f, .sustain = 0.125f, .release = 0.12f,
    .start_frequency = RIV_NOTE_A6, .end_frequency = RIV_NOTE_A6,
    .amplitude = .25f, .sustain_level = 0.3f, .duty_cycle = 0.5f, .pan = 0,
};

riv_waveform_desc max_fuel = {
    .type = RIV_WAVEFORM_TRIANGLE,
    .attack = 0.025f, .decay = 0.075f, .sustain = 0.125f, .release = 0.12f,
    .start_frequency = RIV_NOTE_A6, .end_frequency = RIV_NOTE_E7,
    .amplitude = .5f, .sustain_level = 0.3f, .duty_cycle = 0.5f, .pan = 0,
};

void sfx_explosion()
{
    riv_waveform(&explosion);
}

uint64_t last_frame_move = 0ULL;
void sfx_move()
{
    if((last_frame_move + WAVEFORM_FRAMES) < riv->frame)
    {
        riv_waveform(&move);
        last_frame_move = riv->frame;
    }
}

void sfx_shoot()
{
    riv_waveform(&shoot);
}

void sfx_fuel()
{
    riv_waveform(&fuel);
}

uint64_t last_frame_max_fuel = 0ULL;
void sfx_max_fuel()
{
    if((last_frame_max_fuel + WAVEFORM_FRAMES) < riv->frame)
    {
        riv_waveform(&max_fuel);
        last_frame_max_fuel = riv->frame;
    }
}



Game g;
Player p;
Level l;

void init_riv()
{
    // Set screen size and default frame rate
    riv->width = SCREEN_WIDTH;
    riv->height = SCREEN_HEIGHT;
    riv->target_fps = TARGET_FPS;

    // Load sprites
    riv_make_spritesheet(riv_make_image("rivesraid.png", 0), TILE_SIZE, TILE_SIZE);
}

void update()
{
    if(g.started)
    {
        update_game(&g);
    }
    else
    {
        if (riv->key_toggle_count > 0) 
        {
            g.started = true;
        }
    }
}

void draw_title_screen()
{
    // Draw title
    riv_draw_text(
        "river rives",            // text to draw
        RIV_SPRITESHEET_FONT_5X7, // sprite sheet id of the font
        RIV_CENTER,               // anchor point on the text bounding box
        CENTER_X,                 // anchor x
        CENTER_Y,                 // anchor y
        2,                        // text size multiplier
        RIV_COLOR_LIGHTBLUE       // text color
    );

    int blink = TARGET_FPS / 5;
    uint32_t color = (riv->frame % blink < (blink/2)) ? RIV_COLOR_LIGHTBLUE : RIV_COLOR_DARKGREEN;
    // Draw press to start
    riv_draw_text("PRESS TO START", RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, CENTER_X, CENTER_Y + 16, 1, color);
}

void draw()
{
    riv_clear(RIV_COLOR_DARKSLATE);

    if(g.started)
    {
        draw_game(&g);
    }
    else
    {
        draw_title_screen();
    }
}

int main() 
{
    init_riv();
    init_game(&g, &p, &l);

    do 
    {
        update();
        draw();
    } while(riv_present());

    return 0;
}