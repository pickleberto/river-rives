#pragma once

#include <riv.h>

typedef struct player
{
    riv_vec2i pos;
    bool isDead;
} Player;

void init_player(Player* p);
void update_player(Player* p);
void draw_player(Player* p);