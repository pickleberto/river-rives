#include <riv.h>

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 256
#define TARGET_FPS 60

void init_riv()
{
    // Set screen size and default frame rate
    riv->width = SCREEN_WIDTH;
    riv->height = SCREEN_HEIGHT;
    riv->target_fps = TARGET_FPS;
}

int main() 
{
    init_riv();

    do {
        // Draw title
        riv_draw_text(
            "rives raid",             // text to draw
            RIV_SPRITESHEET_FONT_5X7, // sprite sheet id of the font
            RIV_CENTER,               // anchor point on the text bounding box
            SCREEN_WIDTH/2,           // anchor x
            SCREEN_HEIGHT/2,          // anchor y
            2,                        // text size multiplier
            RIV_COLOR_LIGHTBLUE       // text color
        );
    } while(riv_present());
    return 0;
}