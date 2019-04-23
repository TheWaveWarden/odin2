
#pragma once
#include "GlobalIncludes.h"
#include <cstring>

// contains all modulation destinations in an oscillator (all types)
struct ModDestOsc {
  float pitch_linear;
  float pitch_exponential;
  float vol;
  // float drift;
  float pulse_width;
  float carrier_ratio;
  float modulator_ratio;
  float fm_amount;
  float hp_freq;
  float lp_freq;
  float x;
  float y;
  float position;
  float detune;
  float spread;
  float arp_speed;
};

// contains all modulation destinations in a filter (all types)
struct ModDestFilter {
  float freq;
  float res;
  float drive;
  float gain;
  float env_amount;
  float vel_amount;
  float kbd_amount;
  float SEM_transition;
  float formant_transition;
};

// contains all modulation destinations for an envelope
struct ModDestADSR {
  float attack;
  float decay;
  float sustain;
  float release;
};

// contains all modulation destination within an envelope
struct ModDestLFO {
  float freq;
};

struct ModDestAmp {
  float gain;
  float pan;
  float vel;
};

struct ModDestDistortion {
  float threshold;
  float drywet;
};

struct ModDestDelay {
  float time;
  float feedback;
  float hp_freq;
  float ducking;
  float dry;
  float wet;
};

struct ModDestPhaser {
  float amount;
  float freq;
  float drywet;
};

struct ModDestFlanger {
  float amount;
  float freq;
  float drywet;
};

struct ModDestChorus {
  float amount;
  float freq;
  float drywet;
};

struct ModDestMisc {
  float master;
  float glide;
};

struct ModDestVoice {
  ModDestOsc osc[3];
  ModDestFilter filter[2];
  ModDestADSR adsr[4];
  ModDestLFO lfo[4];
};

struct ModDestinations {
  ModDestVoice voice[VOICES];
  ModDestAmp amp;
  ModDestDistortion distortion;
  ModDestFilter filter3;
  ModDestDelay delay;
  ModDestPhaser phaser;
  ModDestFlanger flanger;
  ModDestChorus chorus;
  ModDestMisc misc;
};

struct ModSourceVoice {
  float* osc[3];
  float* filter[2];
  float* adsr[4];
  float* lfo[4];
  float* MIDI_key;
  float* MIDI_velocity;
};

struct ModSources {
  ModSourceVoice voice[VOICES];
  float* filter3;
  float* modwheel;
  float* pitchwheel;
  float* x;
  float* y;
  float* MIDI_aftertouch;
  float* constant;
};

class ModMatrixRow {
public:
  ModMatrixRow(){}
  //ModMatrixRow(ModSources &p_source, ModSources &p_destination);
  void setSourcesAndDestinations(ModSources* p_source, ModDestinations* p_destination);

  operator bool() const {
    return m_active;
  }

  void applyModulation();


  void setModSource(int p_source);
  void setModDestination(int p_destination);
  void setModScale(int p_scale);

  void setModAmount(float p_mod_amount){
    m_mod_amount = p_mod_amount;
  }

  void setScaleAmount(float p_scale_amount){
    m_scale_amount = p_scale_amount;
  }

private:
  int m_source = 0;
  int m_destination = 0;
  int m_scale = 0;
  bool m_active = false;

  float m_mod_amount = 0.f;
  float m_scale_amount = 0.f;

  float* m_source_value[VOICES];
  float* m_destination_value[VOICES];
  float* m_scale_value[VOICES];

  bool m_destination_poly = false;


  ModSources* m_sources;
  ModDestinations* m_destinations;
};

class ModMatrix {
public:
  ModMatrix(){}
  //ModMatrix(ModSources *p_source, ModDestinations *p_destination);

  void setSourcesAndDestinations(ModSources *p_source, ModDestinations *p_destination);

  void applyModulation();

  void setModSource(int p_row, int p_source);
  void setModDestination(int p_row, int p_destination);
  void setModScale(int p_row, int p_scale);
  void setModAmount(int p_row, float p_mod_amount);
  void setScaleAmount(int p_row, float p_scale_amount);

  void zeroAllSources();
  void zeroAllDestinations();
private:
  ModMatrixRow m_row[MODMATRIX_ROWS];
  ModSources* m_sources;
  ModDestinations* m_destinations;
};