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

riv_waveform_desc shoot = {
    .type = RIV_WAVEFORM_PULSE,
    .attack = 0.05f, .decay = 0.05f, .sustain = 0.15f, .release = 0.075f,
    .start_frequency = RIV_NOTE_A5, .end_frequency = RIV_NOTE_A3,
    .amplitude = .75f, .sustain_level = 0.25f, .duty_cycle = 0.65f, .pan = 0,
};

riv_waveform_desc fuel = {
    .type = RIV_WAVEFORM_TRIANGLE,
    .attack = 0.025f, .decay = 0.075f, .sustain = 0.125f, .release = 0.12f,
    .start_frequency = RIV_NOTE_A6, .end_frequency = RIV_NOTE_A6,
    .amplitude = .25f, .sustain_level = 0.3f, .duty_cycle = 0.5f, .pan = 0,
};

riv_waveform_desc max_fuel = {
    .type = RIV_WAVEFORM_TRIANGLE,
    .attack = 0.025f, .decay = 0.075f, .sustain = 0.125f, .release = 0.12f,
    .start_frequency = RIV_NOTE_A6, .end_frequency = RIV_NOTE_E7,
    .amplitude = .5f, .sustain_level = 0.3f, .duty_cycle = 0.5f, .pan = 0,
};

void sfx_explosion()
{
    riv_waveform(&explosion);
}

uint64_t last_frame_move = 0ULL;
void sfx_move()
{
    if((last_frame_move + WAVEFORM_FRAMES) < riv->frame)
    {
        riv_waveform(&move);
        last_frame_move = riv->frame;
    }
}

void sfx_shoot()
{
    riv_waveform(&shoot);
}

void sfx_fuel()
{
    riv_waveform(&fuel);
}

uint64_t last_frame_max_fuel = 0ULL;
void sfx_max_fuel()
{
    if((last_frame_max_fuel + WAVEFORM_FRAMES) < riv->frame)
    {
        riv_waveform(&max_fuel);
        last_frame_max_fuel = riv->frame;
    }
}