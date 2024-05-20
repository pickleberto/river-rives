#ifndef PLAYER_H
#define PLAYER_H

#include <riv.h>
#include "level.h"
#include "score.h"

typedef struct player
{
    riv_rectf rect;
    bool isDead;
    bool isFinalScreen;

    int sprite_id;
    int flip_x;

    Score* score;
} Player;

void init_player(Player* p, Score* s);
void update_player(Player* p, Level* l);
void draw_player(Player* p);

void kill_player(Player* p);

#endif