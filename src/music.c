#include "music.h"

#include "const.h"

#define MUSIC_SPEED 10
#define M_SUSTAIN .5f
#define M_ATK_REL .15f

#define COMPLETED_MUSIC_SPEED 12
#define GAME_OVER_MUSIC_SPEED 30

riv_note_freq  bass_notes[] = {
  RIV_NOTE_A4, 0, RIV_NOTE_A4, RIV_NOTE_A4, RIV_NOTE_A4, RIV_NOTE_A4, 0, RIV_NOTE_A4, RIV_NOTE_A4, RIV_NOTE_A4, RIV_NOTE_A4, 0, RIV_NOTE_A4, RIV_NOTE_A4, RIV_NOTE_C5, RIV_NOTE_A4,
  RIV_NOTE_C5, 0, RIV_NOTE_C5, RIV_NOTE_C5, RIV_NOTE_C5, RIV_NOTE_C5, 0, RIV_NOTE_C5, RIV_NOTE_C5, RIV_NOTE_C5, RIV_NOTE_C5, 0, RIV_NOTE_C5, RIV_NOTE_C5, RIV_NOTE_D5, RIV_NOTE_C5,
  RIV_NOTE_D5, 0, RIV_NOTE_D5, RIV_NOTE_D5, RIV_NOTE_D5, RIV_NOTE_D5, 0, RIV_NOTE_D5, RIV_NOTE_D5, RIV_NOTE_D5, RIV_NOTE_D5, 0, RIV_NOTE_D5, RIV_NOTE_D5, RIV_NOTE_G4, RIV_NOTE_D5,
  RIV_NOTE_G4, 0, RIV_NOTE_G4, RIV_NOTE_G4, RIV_NOTE_G4, RIV_NOTE_G4, 0, RIV_NOTE_G4, RIV_NOTE_G4, RIV_NOTE_G4, RIV_NOTE_G4, 0, RIV_NOTE_G4, RIV_NOTE_G4, RIV_NOTE_A4, RIV_NOTE_G4,
};

riv_note_freq  rhythm_notes[] = {
  RIV_NOTE_C4, RIV_NOTE_Eb4, RIV_NOTE_G4, RIV_NOTE_Bb4, RIV_NOTE_C4, RIV_NOTE_Eb4, RIV_NOTE_G4, RIV_NOTE_Bb4, RIV_NOTE_C4, RIV_NOTE_Eb4, RIV_NOTE_G4, RIV_NOTE_Bb4, RIV_NOTE_C4, RIV_NOTE_Eb4, RIV_NOTE_G4, RIV_NOTE_Bb4,
  RIV_NOTE_F4, RIV_NOTE_Ab4, RIV_NOTE_C5, RIV_NOTE_D5, RIV_NOTE_F4, RIV_NOTE_Ab4, RIV_NOTE_C5, RIV_NOTE_D5, RIV_NOTE_F4, RIV_NOTE_Ab4, RIV_NOTE_C5, RIV_NOTE_D5, RIV_NOTE_F4, RIV_NOTE_Ab4, RIV_NOTE_C5, RIV_NOTE_D5,
  RIV_NOTE_Eb4, RIV_NOTE_G4, RIV_NOTE_Bb4, RIV_NOTE_C5, RIV_NOTE_Eb4, RIV_NOTE_G4, RIV_NOTE_Bb4, RIV_NOTE_C5, RIV_NOTE_Eb4, RIV_NOTE_G4, RIV_NOTE_Bb4, RIV_NOTE_C5, RIV_NOTE_Eb4, RIV_NOTE_G4, RIV_NOTE_Bb4, RIV_NOTE_C5,
  RIV_NOTE_G4, RIV_NOTE_B4, RIV_NOTE_D5, RIV_NOTE_F5, RIV_NOTE_G4, RIV_NOTE_B4, RIV_NOTE_D5, RIV_NOTE_F5, RIV_NOTE_G4, RIV_NOTE_B4, RIV_NOTE_D5, RIV_NOTE_F5, RIV_NOTE_G4, RIV_NOTE_B4, RIV_NOTE_D5, RIV_NOTE_F5,
};

riv_note_freq lead_notes[] = {
RIV_NOTE_A4, RIV_NOTE_C5, RIV_NOTE_A4, RIV_NOTE_G4, RIV_NOTE_F4, RIV_NOTE_G4, RIV_NOTE_E4, RIV_NOTE_E4, RIV_NOTE_Gb4, RIV_NOTE_G4, RIV_NOTE_B4, RIV_NOTE_A4, RIV_NOTE_G4, RIV_NOTE_Gb4, RIV_NOTE_A4, RIV_NOTE_Gb4,
RIV_NOTE_A4, RIV_NOTE_A4, RIV_NOTE_E4, RIV_NOTE_A4, RIV_NOTE_E5, RIV_NOTE_D5, RIV_NOTE_C5, RIV_NOTE_B4, RIV_NOTE_B4, RIV_NOTE_E4, RIV_NOTE_A4, RIV_NOTE_B4, RIV_NOTE_C5, RIV_NOTE_B4, RIV_NOTE_A4, RIV_NOTE_G4,
RIV_NOTE_A4, RIV_NOTE_C5, RIV_NOTE_A4, RIV_NOTE_G4, RIV_NOTE_F4, RIV_NOTE_G4, RIV_NOTE_E4, RIV_NOTE_E4, RIV_NOTE_Gb4, RIV_NOTE_G4, RIV_NOTE_B4, RIV_NOTE_A4, RIV_NOTE_G4, RIV_NOTE_Gb4, RIV_NOTE_A4, RIV_NOTE_Gb4,
RIV_NOTE_G4, RIV_NOTE_G4, RIV_NOTE_E4, RIV_NOTE_D4, RIV_NOTE_C4, RIV_NOTE_E4, RIV_NOTE_A4, RIV_NOTE_A4, RIV_NOTE_G4, RIV_NOTE_E4, RIV_NOTE_E4, RIV_NOTE_D4, RIV_NOTE_C4, RIV_NOTE_D4, RIV_NOTE_C4, RIV_NOTE_C4,
};

riv_note_freq  completed_notes[] = {
  RIV_NOTE_A4, RIV_NOTE_A4, RIV_NOTE_Bb4, 0, RIV_NOTE_A4, RIV_NOTE_C5, 0, RIV_NOTE_G4, 0, 0, RIV_NOTE_A4, RIV_NOTE_C5, RIV_NOTE_A4, RIV_NOTE_G4, RIV_NOTE_F4, 0,
};

riv_note_freq  game_over_notes[] = {
  RIV_NOTE_C3, RIV_NOTE_B2, RIV_NOTE_G2, RIV_NOTE_E2, 
};

float durations[] ={
    1.f, 1.f, 1.f, 2.f, 
};


uint64_t next_frame = 0ULL;
int notes_index = 0;
bool play_music_notes(riv_note_freq notes[], int index, riv_waveform_type wave_type)
{
    if(riv->frame >= next_frame)
    {
        riv_waveform_desc current_note = {
            .type = wave_type,
            .attack = M_ATK_REL, .release = M_ATK_REL, .sustain_level = M_SUSTAIN,
            .duty_cycle = 0.75, .amplitude = 0.2f, 
            .start_frequency = notes[index], 
        };
    
        riv_waveform(&current_note);
        return true;
    }
    return false;
}

bool play_bass_notes(riv_note_freq notes[], int index)
{
    if(riv->frame >= next_frame)
    {
        riv_waveform_desc current_note = {
            .type = RIV_WAVEFORM_TRIANGLE,
            .attack = M_ATK_REL, .release = M_ATK_REL, .sustain_level = M_SUSTAIN,
            .duty_cycle = 0.75, .amplitude = 0.15f, 
            .start_frequency = notes[index],
        };
    
        riv_waveform(&current_note);
        return true;
    }
    return false;
}

int note_count = 0;
void play_music()
{
    if(play_bass_notes(bass_notes, notes_index))
    {
        if((note_count / N_ELEMS(rhythm_notes)) % 3 == 2)
        {
            play_music_notes(lead_notes, notes_index, RIV_WAVEFORM_SAWTOOTH);
        }else if((note_count / N_ELEMS(rhythm_notes)) % 3 == 1)
        {
            play_music_notes(rhythm_notes, notes_index, RIV_WAVEFORM_SINE);
        }
        notes_index = (notes_index + 1) % N_ELEMS(rhythm_notes);
        next_frame = riv->frame + MUSIC_SPEED;
        note_count ++;
    }
}

int completed_index = 0;
void play_completion()
{
    if(completed_index < N_ELEMS(completed_notes) && play_music_notes(completed_notes, completed_index, RIV_WAVEFORM_PULSE) )
    {
        next_frame = riv->frame + COMPLETED_MUSIC_SPEED;
        completed_index ++;
    }
}

int game_over_notes_index = 0;
void play_game_over()
{
    if(riv->frame >= next_frame && game_over_notes_index < N_ELEMS(game_over_notes))
    {
        int d_index = notes_index % 16;
        riv_waveform_desc current_note = {
            .type = RIV_WAVEFORM_SQUARE,
            .attack = .5f, .release = .5f, .sustain_level = durations[game_over_notes_index],
            .duty_cycle = 0.25, .amplitude = 0.1f, 
            .start_frequency = game_over_notes[game_over_notes_index], 
        };
    
        riv_waveform(&current_note);
        next_frame = riv->frame + GAME_OVER_MUSIC_SPEED * durations[game_over_notes_index++];
    }
}