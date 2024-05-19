#include "sounds.h"

#include <riv.h>
#include "const.h"

#define WAVEFORM_FRAMES (uint64_t)(.5f * TARGET_FPS)

riv_waveform_desc explosion = {
    .type = RIV_WAVEFORM_NOISE,
    .attack = 0.025f, .decay = 0.1f, .sustain = 0.5f, .release = 0.025f,
    .start_frequency = RIV_NOTE_A2, .end_frequency = RIV_NOTE_A0,
    .amplitude = 1.f, .sustain_level = 0.5f, .duty_cycle = 0.5f, .pan = 0,
};

riv_waveform_desc move = {
    .type = RIV_WAVEFORM_NOISE,
    .attack = 0.025f, .decay = 0.1f, .sustain = 0.5f, .release = 0.025f,
    .start_frequency = RIV_NOTE_G1, .end_frequency = RIV_NOTE_G1,
    .amplitude = 0.5f, .sustain_level = 0.5f, .duty_cycle = 0.5f, .pan = 0,
};

void sfx_explosion()
{
    riv_waveform(&explosion);
}

uint64_t last_frame = 0ULL;
void sfx_move()
{
    if((last_frame + WAVEFORM_FRAMES) < riv->frame)
    {
        riv_waveform(&move);
        last_frame = riv->frame;
    }
}