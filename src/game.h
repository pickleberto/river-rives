#pragma once

#include <riv.h>
#include "player.h"
#include "level.h"

typedef struct game
{
    bool started;
    bool game_over;
    Player* player;
    Level* level;
} Game;


void init_game(Game* g, Player* p, Level* l);
void update_game(Game* g);
void draw_game(Game* g);

