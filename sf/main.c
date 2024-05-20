#include "headers.h"

// defines
#define BULLET_WIDTH 2
#define BULLET_HEIGHT 4
#define BULLET_SPEED 5

#define SCALE_X 1
#define SCALE_Y 1

#define TILES_PER_SECOND ((float) TILE_SIZE/TARGET_FPS)
#define BASE_SPEED (2.f * TILES_PER_SECOND)
#define SHOOT_COOLDOWN_FRAMES (uint64_t) (0.25f * TARGET_FPS)

#define ACCELERATION 2.5f

#define MAX_FUEL (10 * TARGET_FPS)
#define FUEL_WIDTH (2 * TILE_SIZE)
#define FUEL_HEIGHT (HALF_TILE)

#define WAVEFORM_FRAMES (uint64_t)(.5f * TARGET_FPS)

// global vars
Score s;
BulletPool bullets;
Game g;
Player p;
Level l;

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

    if(tile_collision(b->rect, *l, s))
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
    if(g->game_over) return;
    
    update_level(g->level);
    update_player(g->player, g->level);

    if(player_tile_collision(g->player->rect, *g->level, g->player->score) || (g->player->score->fuel <= 0))
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
}

void draw_game(Game* g)
{
    draw_level(g->level);
    draw_player(g->player);
    
    if(g->game_over)
    {
        riv_draw_text("game over", RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, CENTER_X, CENTER_Y, 2, RIV_COLOR_RED);
    }
}

void init_level(Level* l)
{
    l->screen_speed = BASE_MAP_SPEED; // pixels per frame
    l->map_offset = 0;
    
    l->min_y = (FULL_TILES_Y - SCREEN_TILES_Y) - 1;
    l->max_y = FULL_TILES_Y;
}

void update_level(Level* l)
{
    if(l->min_y <= 0) return; // lock map on final screen

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
    }
    else
    {
        l->map_offset = (l->map_offset + l->screen_speed);
    }
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
}

bool screen_tile_collision(float x, float y, Level l, Score* s)
{
    int tile_x = x / SCREEN_TILES_X;
    int tile_y = ((y - l.map_offset) / SCREEN_TILES_Y) + l.min_y + 1;

    // remember full_level_map is inverted
    if(full_level_map[tile_y][tile_x] >= OBSTACLE)
    {
        full_level_map[tile_y][tile_x] = RIVER; //if destructable, then destroy it
        add_obstacle(s);
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
        full_level_map[tile_y][tile_x] = RIVER; //if destructable, then destroy it
        add_obstacle(s);
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
    if(p->isDead) return;
    
    update_horizontal(p);
    update_fire(p, l);
    update_vertical(p, l);
    update_score(p->score);   
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
    draw_score(p->score);
}

void kill_player(Player* p)
{
    p->isDead = true;
    sfx_explosion();
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


void init_score(Score* s)
{
    s->score = 0;
    s->ticks = 0;
    s->obstacles_destroyed = 0;
    s->fuel = MAX_FUEL;
}

void update_score(Score* s) 
{
    s->ticks++;
    s->fuel--;
    s->score = (s->obstacles_destroyed * 100) + s->fuel - (s->ticks / 10);
    riv->outcard_len = riv_snprintf((char*)riv->outcard, RIV_SIZE_OUTCARD, 
        "JSON{\"score\":%d,\"obstacles\":%d,\"ticks\":%d}", s->score, s->obstacles_destroyed, s->ticks);
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
        "rives raid",             // text to draw
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