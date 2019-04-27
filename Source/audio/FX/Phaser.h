#pragma once

#include "../Filters/BiquadAllpass.h"
#include <cmath>

#define PHASER_MAX_LFO_AMPLITUDE 4000

class Phaser {
public:
  Phaser();
  ~Phaser();

  inline void setSamplerate(float p_samplerate) {
    m_samplerate = p_samplerate;

    m_AP1.setSamplerate(p_samplerate);
    m_AP2.setSamplerate(p_samplerate);
    m_AP3.setSamplerate(p_samplerate);
    m_AP4.setSamplerate(p_samplerate);
    m_AP5.setSamplerate(p_samplerate);
    m_AP6.setSamplerate(p_samplerate);

	setLFOFreq(1.f);
  }

  inline void reset() {
    m_AP1.reset();
    m_AP2.reset();
    m_AP3.reset();
    m_AP4.reset();
    m_AP5.reset();
    m_AP6.reset();
    m_index_sine = 0;
  }

  float doPhaser(float p_input);
  void setAmount(float p_amount);

  inline void setRadiusBase(float p_radius_base) {
    m_radius_base = p_radius_base;
  }

  inline void setLFOFreq(float p_freq) {
    m_increment_sine = 2 * p_freq / m_samplerate;
  }

  inline void setLFOAmplitude(float p_amplitude) {
    m_LFO_amplitude = p_amplitude;
  }

  inline void setBaseFreq(float p_base_freq) { m_base_freq = p_base_freq; }

  void setFreqModPointer(float *p_pointer) { m_freq_mod = p_pointer; }
  void setAmountModPointer(float *p_pointer) { m_amount_mod = p_pointer; }
  void setDryWetModPointer(float *p_pointer) { m_drywet_mod = p_pointer; }

protected:
  float *m_freq_mod;
  float *m_amount_mod;
  float *m_drywet_mod;

  inline void incrementLFO() {
    float increment_modded = m_increment_sine;
    if (*m_freq_mod) {
      increment_modded *= pow(4, *m_freq_mod);
    }
    m_index_sine += increment_modded;
    while (m_index_sine > 1) {
      m_index_sine -= 1.f;
      m_LFO_sign *= -1;
    }
  }

  inline float doLFO() {
    // cheap approximation by parabola
    return 4 * (m_index_sine * (1 - m_index_sine)) * m_LFO_sign;
  }
  void setRadius(float p_radius);
  void setFrequency(float p_frequency);

  float m_amount;
  float m_base_freq;

  BiquadAllpass m_AP1;
  BiquadAllpass m_AP2;
  BiquadAllpass m_AP3;
  BiquadAllpass m_AP4;
  BiquadAllpass m_AP5;
  BiquadAllpass m_AP6;

  int m_LFO_sign =
      1; // switches from the positive to the negative proportion of the LFO
  float m_LFO_amplitude;
  float m_index_sine = 0;
  float m_increment_sine;

  float m_radius_base;
  float m_samplerate;
};
