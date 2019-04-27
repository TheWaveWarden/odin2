/*
  ==============================================================================

    Voice.h
    Created: 30 Mar 2019 1:52:30am
    Author:  frot

  ==============================================================================
*/
#pragma once

#include "GlobalIncludes.h"
#include "audio/Oscillators/AnalogOscillator.h"
#include "audio/Oscillators/ChiptuneOscillator.h"
#include "audio/Oscillators/FMOscillator.h"
#include "audio/Oscillators/MultiOscillator.h"
#include "audio/Oscillators/NoiseOscillator.h"
#include "audio/Oscillators/VectorOscillator.h"
#include "audio/Oscillators/WavetableOsc2D.h"
#include "audio/Oscillators/LFO.h"


// wavedraw
// chipdraw
// specdraw
#include "audio/Filters/CombFilter.h"
#include "audio/Filters/DiodeFilter.h"
#include "audio/Filters/FormantFilter.h"
#include "audio/Filters/Korg35Filter.h"
#include "audio/Filters/LadderFilter.h"
#include "audio/Filters/SEMFilter12.h"
#include "audio/Filters/SEMFilter24.h"

#include "audio/ADSR.h"

class VoiceManager {
public:
  // returns free voice index or steals one and sets it busy
  int getVoice() {
    for (int i = 0; i < VOICES; ++i) {
      if (!voice_busy[i]) {
        voice_busy[i] = true;
        DBG("Voice manager returned voice " + std::to_string(i));
        return i;
      }
    }
    // TODO all voices busy, steal
    DBG("Voice manager returned voice 0");    
    return 0;
  }

  // marks a voice as free again
  void freeVoice(int p_voice) { 
    voice_busy[p_voice] = false; 
    DBG("Voice manager freed voice " + std::to_string(p_voice));
    }

protected:
  bool voice_busy[VOICES] = {false}; // is voice busy
};

// one voice of the polyphonic voices, i.e. everything up to the amplifier
struct Voice {

  // implicit conversion to bool
  operator bool() const { return m_voice_active; }

  Voice() {
    env[0].onEnvelopeEnd = [&]() { onEnvelopeEnd(); };
  }

  float MIDINoteToFreq(int p_MIDI_note) {
    return 27.5f * pow(2.f, (float)(p_MIDI_note - 21) / 12.f);
  }

  void start(int p_MIDI_key, int p_MIDI_velocity) {
    reset();
    setOscBaseFreq(MIDINoteToFreq(p_MIDI_key));
    setFilterMIDIValues(p_MIDI_key, p_MIDI_velocity);
    m_voice_active = true;
    DBG("Started voice");
    m_MIDI_key = p_MIDI_key;
    MIDI_key_mod_source = (float)p_MIDI_key / 127.f;
    MIDI_velocity_mod_source = (float)p_MIDI_velocity / 127.f;
  }

  // starts release on envelopes if this is the key that was pressed
  // returns true if the voice was actually stopped
  bool keyUp(int p_MIDI_key) {
    if (m_MIDI_key == p_MIDI_key) {
      env[1].startRelease();
      env[2].startRelease();
      env[3].startRelease();
      env[4].startRelease();
      //TODO REMOVE HAACK
      m_voice_active = false;
      return true;
    }
    return false;
  }

  void setOctave(int p_octave, int p_osc) {
    analog_osc[p_osc].m_octave = p_octave;
    wavedraw_osc[p_osc].m_octave = p_octave;
    chipdraw_osc[p_osc].m_octave = p_octave;
    specdraw_osc[p_osc].m_octave = p_octave;
    wavetable_osc[p_osc].m_octave = p_octave;
    multi_osc[p_osc].m_octave = p_octave;
    vector_osc[p_osc].m_octave = p_octave;
    chiptune_osc[p_osc].m_octave = p_octave;
    fm_osc[p_osc].m_octave = p_octave;
  }

  void setSemitones(int p_semi, int p_osc) {
    analog_osc[p_osc].m_semitones = p_semi;
    wavedraw_osc[p_osc].m_semitones = p_semi;
    chipdraw_osc[p_osc].m_semitones = p_semi;
    specdraw_osc[p_osc].m_semitones = p_semi;
    wavetable_osc[p_osc].m_semitones = p_semi;
    multi_osc[p_osc].m_semitones = p_semi;
    vector_osc[p_osc].m_semitones = p_semi;
    chiptune_osc[p_osc].m_semitones = p_semi;
    fm_osc[p_osc].m_semitones = p_semi;
  }

  void setFinetune(float p_fine, int p_osc) {
    analog_osc[p_osc].m_cent = p_fine;
    wavedraw_osc[p_osc].m_cent = p_fine;
    chipdraw_osc[p_osc].m_cent = p_fine;
    specdraw_osc[p_osc].m_cent = p_fine;
    wavetable_osc[p_osc].m_cent = p_fine;
    multi_osc[p_osc].m_cent = p_fine;
    vector_osc[p_osc].m_cent = p_fine;
    chiptune_osc[p_osc].m_cent = p_fine;
    fm_osc[p_osc].m_cent = p_fine;
  }

  void setReset(bool p_reset, int p_osc) {
    analog_osc[p_osc].m_reset = p_reset;
    wavedraw_osc[p_osc].m_reset = p_reset;
    chipdraw_osc[p_osc].m_reset = p_reset;
    specdraw_osc[p_osc].m_reset = p_reset;
    wavetable_osc[p_osc].m_reset = p_reset;
    multi_osc[p_osc].m_reset = p_reset;
    vector_osc[p_osc].m_reset = p_reset;
    chiptune_osc[p_osc].m_reset = p_reset;
    fm_osc[p_osc].m_reset = p_reset;
  }

  void setOscBaseFreq(float p_freq) {
    for (int osc = 0; osc < 3; ++osc) {
      analog_osc[osc].setBaseFrequency(p_freq);
      wavedraw_osc[osc].setBaseFrequency(p_freq);
      chipdraw_osc[osc].setBaseFrequency(p_freq);
      specdraw_osc[osc].setBaseFrequency(p_freq);
      wavetable_osc[osc].setBaseFrequency(p_freq);
      multi_osc[osc].setBaseFrequency(p_freq);
      vector_osc[osc].setBaseFrequency(p_freq);
      chiptune_osc[osc].setBaseFrequency(p_freq);
      fm_osc[osc].setBaseFrequency(p_freq);
    }
  }

  void reset() {
    for (int osc = 0; osc < 3; ++osc) {
      analog_osc[osc].reset();
      wavetable_osc[osc].reset();
      wavedraw_osc[osc].reset();
      chipdraw_osc[osc].reset();
      specdraw_osc[osc].reset();
      multi_osc[osc].reset();
      vector_osc[osc].reset();
      chiptune_osc[osc].reset();
      fm_osc[osc].reset();
      wavedraw_osc[osc].reset();
      specdraw_osc[osc].reset();
      chipdraw_osc[osc].reset();
    }
    for(int mod = 0; mod < 4; ++mod){
      lfo[mod].reset();
    }
    for (int fil = 0; fil < 2; ++fil) {
      ladder_filter[fil].reset();
      diode_filter[fil].reset();
      formant_filter[fil].reset();
      korg_filter[fil].reset();
      SEM_filter_12[fil].reset();
    }
  }

  void setFilterMIDIValues(int p_MIDI_note, int p_MIDI_vel) {
    //shift note here so the lowest note possible is 21
    p_MIDI_note -= 21;
    p_MIDI_note = p_MIDI_note < 0 ? 0 : p_MIDI_note;
    
    for (int fil = 0; fil < 3; ++fil) {
      ladder_filter[fil].m_MIDI_note = p_MIDI_note;
      diode_filter[fil].m_MIDI_note = p_MIDI_note;
      korg_filter[fil].m_MIDI_note = p_MIDI_note;
      SEM_filter_24[fil].m_MIDI_note = p_MIDI_note;
      SEM_filter_12[fil].m_MIDI_note = p_MIDI_note;
      comb_filter[fil].m_MIDI_note = p_MIDI_note;

      ladder_filter[fil].m_MIDI_velocity = p_MIDI_vel;
      diode_filter[fil].m_MIDI_velocity = p_MIDI_vel;
      korg_filter[fil].m_MIDI_velocity = p_MIDI_vel;
      SEM_filter_24[fil].m_MIDI_velocity = p_MIDI_vel;
      SEM_filter_12[fil].m_MIDI_velocity = p_MIDI_vel;
      comb_filter[fil].m_MIDI_velocity = p_MIDI_vel;
      formant_filter[fil].m_MIDI_velocity = p_MIDI_vel;
    }
  }

  void setKbd(float p_kbd, int p_fil) {
    ladder_filter[p_fil].m_kbd_mod_amount = p_kbd;
    SEM_filter_12[p_fil].m_kbd_mod_amount = p_kbd;
    SEM_filter_24[p_fil].m_kbd_mod_amount = p_kbd;
    korg_filter[p_fil].m_kbd_mod_amount = p_kbd;
    diode_filter[p_fil].m_kbd_mod_amount = p_kbd;
    comb_filter[p_fil].m_kbd_mod_amount = p_kbd;
  }

  void setVelModAmount(float p_vel, int p_fil) {
    ladder_filter[p_fil].m_vel_mod_amount = p_vel;
    SEM_filter_12[p_fil].m_vel_mod_amount = p_vel;
    SEM_filter_24[p_fil].m_vel_mod_amount = p_vel;
    korg_filter[p_fil].m_vel_mod_amount = p_vel;
    diode_filter[p_fil].m_vel_mod_amount = p_vel;
    comb_filter[p_fil].m_vel_mod_amount = p_vel;
    formant_filter[p_fil].m_vel_mod_amount = p_vel;
  }

  void setSaturation(float p_sat, int p_fil){
    ladder_filter[p_fil].m_overdrive = p_sat;
    SEM_filter_12[p_fil].m_overdrive = p_sat;
    SEM_filter_24[p_fil].m_overdrive = p_sat;
    korg_filter[p_fil].m_overdrive = p_sat;
    diode_filter[p_fil].m_overdrive = p_sat;
  }

  void setEnvModAmount(float p_env, int p_fil) {
    //set as quadratic
    ladder_filter[p_fil].m_env_mod_amount = p_env;
    SEM_filter_12[p_fil].m_env_mod_amount = p_env;
    SEM_filter_24[p_fil].m_env_mod_amount = p_env;
    korg_filter[p_fil].m_env_mod_amount = p_env;
    diode_filter[p_fil].m_env_mod_amount = p_env;
    comb_filter[p_fil].m_env_mod_amount = p_env;
    formant_filter[p_fil].m_env_mod_amount = p_env;
  }

  void setFilterEnvValue(float p_env){    
    ladder_filter[0].m_env_value = p_env;
    SEM_filter_12[0].m_env_value = p_env;
    SEM_filter_24[0].m_env_value = p_env;
    korg_filter[0].m_env_value = p_env;
    diode_filter[0].m_env_value = p_env;
    comb_filter[0].m_env_value = p_env;
    formant_filter[0].m_env_value = p_env;

    ladder_filter[1].m_env_value = p_env;
    SEM_filter_12[1].m_env_value = p_env;
    SEM_filter_24[1].m_env_value = p_env;
    korg_filter[1].m_env_value = p_env;
    diode_filter[1].m_env_value = p_env;
    comb_filter[1].m_env_value = p_env;
    formant_filter[1].m_env_value = p_env;
  }

  void setFilterFreq(float p_freq, int p_fil) {
    //note: not used right now since control is smoothed
    ladder_filter[p_fil].m_freq_base = p_freq;
    SEM_filter_12[p_fil].m_freq_base = p_freq;
    SEM_filter_24[p_fil].m_freq_base = p_freq;
    korg_filter[p_fil].m_freq_base = p_freq;
    diode_filter[p_fil].m_freq_base = p_freq;
    comb_filter[p_fil].setCombFreq(p_freq);
  }

  void setFilterRes(float p_res, int p_fil) {
    ladder_filter[p_fil].setResControl(p_res);
    SEM_filter_12[p_fil].setResControl(p_res);
    SEM_filter_24[p_fil].setResControl(p_res);
    korg_filter[p_fil].setResControl(p_res);
    diode_filter[p_fil].setResControl(p_res);
    comb_filter[p_fil].setResonance(p_res);
  }

  void setSampleRate(float p_samplerate){
    env[0].setSamplerate(p_samplerate);
    env[1].setSamplerate(p_samplerate);
    env[2].setSamplerate(p_samplerate);
    env[3].setSamplerate(p_samplerate);
  }

  // oscs
  AnalogOscillator analog_osc[3];
  WavetableOsc2D wavetable_osc[3];
  MultiOscillator multi_osc[3];
  VectorOscillator vector_osc[3];
  ChiptuneOscillator chiptune_osc[3];
  FMOscillator fm_osc[3];
  NoiseOscillator noise_osc[3];
  WavetableOsc1D wavedraw_osc[3];
  WavetableOsc1D chipdraw_osc[3];
  WavetableOsc1D specdraw_osc[3];


  // filter
  LadderFilter ladder_filter[2];
  SEMFilter12 SEM_filter_12[2];
  SEMFilter24 SEM_filter_24[2];
  Korg35Filter korg_filter[2];
  DiodeFilter diode_filter[2];
  FormantFilter formant_filter[2];
  CombFilter comb_filter[2];
  // ADSRs
  ADSREnvelope env[4];
  LFO lfo[4];

  // LFOs
  // todo

  //modulation values
  float MIDI_key_mod_source = 0.f;
  float MIDI_velocity_mod_source = 0.f;

  // called when the envelope ends to signal voice end to voice manager
  std::function<void()> onEnvelopeEnd = []() {};
  bool m_voice_active = false;
  int m_MIDI_key = 0;
};
