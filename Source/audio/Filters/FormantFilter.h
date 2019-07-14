#pragma once

#include "Filter.h"
#include "BiquadResonator.h"

#define FORMANT_DB_AT_88KHZ -11
#define FORMANT_DB_AT_192KHZ -18

class FormantFilter : public Filter {
public:
  FormantFilter();
  ~FormantFilter();

  void update() override;
  double doFilter(double p_input) override;
  void reset() override;
  void setTransition(float p_trans);
  void setVowelLeft(int p_vowel) {
    m_vowel_left = p_vowel;
    updateParabolas();
  }
  void setVowelRight(int p_vowel) {
    m_vowel_right = p_vowel;
    updateParabolas();
  }

  void setVolModPointer(float *p_pointer) { m_vol_mod = p_pointer; }
  void setTransitionModPointer(float *p_pointer) {
    m_transition_mod = p_pointer;
  }

  void setSampleRate(double p_sr) override{
    Filter::setSampleRate(p_sr);
    m_resonator1.setSampleRate(p_sr);
    m_resonator2.setSampleRate(p_sr);

    //higher sample rates make filter louder so we use a scalar
    //for 44.1khz is 1 for 88.2khz is FORMANT_GAIN_AT_88KHZ
    float m;
    float b;
    if(p_sr < 88200){
      m = (FORMANT_DB_AT_88KHZ) / (44100.f);
      b = -44100.f * m;
    } else {
      m = (FORMANT_DB_AT_192KHZ - FORMANT_DB_AT_88KHZ) / (192600.f - 88200.f);
      b = FORMANT_DB_AT_88KHZ - 88200.f * m;
    }

    float samplerate_db_compenstaion = m * p_sr + b;
    m_samplerate_gain_compensation = Decibels::decibelsToGain(samplerate_db_compenstaion);

    DBG("SAMPLERATEFACTOR: ");
    DBG(samplerate_db_compenstaion);
    DBG(m_samplerate_gain_compensation);
  }

  float m_MIDI_velocity = 0.f;

protected:
  float *m_vol_mod;
  float *m_transition_mod;
  float m_zero_dummy;

  void updateParabolas();

  float m_transition = 0.f;
  int m_vowel_left = 0;
  int m_vowel_right = 1;

  float m_formant_list[8][2] = {
      {1000.f, 1400.f}, // A
      {500.f, 2300.f},  // E
      {320.f, 3200.f},  // I
      {500.f, 1000.f}, // O
      {320.f, 800.f},  // U
      {700.f, 1800.f },  // Ä
      {500.f, 1500.f},   // Ö
      {320.f, 1650.f}   // Ü
  };

  // for parabola interpolation
  float m_a0;
  float m_b0;
  float m_c0;
  float m_a1;
  float m_b1;
  float m_c1;

  float m_samplerate_gain_compensation = 1.f;

  BiquadResonator m_resonator1;
  BiquadResonator m_resonator2;
};
