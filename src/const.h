#pragma once

#define TARGET_FPS 60

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 256

#define CENTER_X (SCREEN_WIDTH / 2)
#define CENTER_Y (SCREEN_HEIGHT / 2)

#define TILE_SIZE 16
#define HALF_TILE (TILE_SIZE / 2)

#define SCREEN_TILES_X (SCREEN_WIDTH / TILE_SIZE)
#define SCREEN_TILES_Y (SCREEN_HEIGHT / TILE_SIZE)

#define GAME_SPRITESHEET 1

#define EXPLOSION_SPRITE_ID 6
#define ANIM_SPRITES 2
#define ANIM_RATE (TARGET_FPS / 10)

#define SPRITE_SCALE_X 1
#define SPRITE_SCALE_Y 1

#define NULL_POINTER ( (void *) 0)

#define N_ELEMS(x)  (sizeof(x) / sizeof((x)[0]))
