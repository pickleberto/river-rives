#include <riv.h>
#include "const.h"
#include "game.h"

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