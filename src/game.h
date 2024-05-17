#pragma once

#include <riv.h>
#include "player.h"

typedef struct game
{
    bool started;
    Player* player;
} Game;


void init_game(Game* g, Player* p);
void update_game(Game* g);
void draw_game(Game* g);
