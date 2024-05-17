#include "sounds.h"

#include <riv.h>

riv_waveform_desc explosion = {
    .type = RIV_WAVEFORM_NOISE,
    .attack = 0.025f, .decay = 0.1f, .sustain = 0.5f, .release = 0.025f,
    .start_frequency = RIV_NOTE_A2, .end_frequency = RIV_NOTE_A0,
    .amplitude = 0.25f, .sustain_level = 0.5f, .duty_cycle = 0.5f, .pan = 0,
};

void sfx_explosion()
{
    riv_waveform(&explosion);
}

void sfx_move()
{

}