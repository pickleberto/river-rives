#pragma once

#include <riv.h>

typedef struct player
{
    riv_rectf rect;
    bool isDead;

    int sprite_id;
    int flip_x;
} Player;

void init_player(Player* p);
void update_player(Player* p);
void draw_player(Player* p);

void kill_player(Player* p);