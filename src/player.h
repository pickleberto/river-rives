#pragma once

#include <riv.h>
#include "level.h"

typedef struct player
{
    riv_rectf rect;
    bool isDead;
    bool isFinalScreen;

    int sprite_id;
    int flip_x;
} Player;

void init_player(Player* p);
void update_player(Player* p, Level* l);
void draw_player(Player* p);

void kill_player(Player* p);