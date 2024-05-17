#pragma once

#include <riv.h>

typedef struct game
{
    bool started;
} Game;


void init_game(Game* g);
void update_game();
void draw_game();
