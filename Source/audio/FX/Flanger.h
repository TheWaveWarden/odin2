/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 - 2021 TheWaveWarden
**
** Odin 2 is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Odin 2 is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/

#pragma once

#include "../Filters/CombFilter.h"

#define FLANGER_LFO_MAX_RANGE 0.0095f // 0.001 to 0.02seconds

class Flanger : public CombFilter {
public:
  Flanger();
  virtual ~Flanger();

  inline void reset() override {
    CombFilter::reset();
    resetLFO();
  }

  inline void resetLFO(){
    m_index_sine = m_LFO_reset_pos;
  }

  float doFlanger(float p_input);

  inline void setBaseTime(float p_base_time) { m_base_time = p_base_time; }

  inline void setLFOFreq(float p_LFO_freq) {
    m_increment_sine = p_LFO_freq / m_samplerate * 2;
    m_LFO_freq = p_LFO_freq;
  }

  void setSampleRate(float p_samplerate) override { 
  //DBG("setsamplerate flanger");
    m_samplerate = p_samplerate;
    setLFOFreq(m_LFO_freq);
  }

  void setFeedback(float p_feedback) {
    p_feedback = p_feedback > 0.98 ? 0.98 : p_feedback;
    p_feedback = p_feedback < -0.98 ? -0.98 : p_feedback;
    setResonance(p_feedback);
  }

  inline void setLFOAmount(float p_LFO_amount) { m_LFO_amount = p_LFO_amount; }

  inline void setDryWet(float p_dry_wet) { m_dry_wet = p_dry_wet; }
  void setFreqModPointer(float *p_pointer) { m_freq_mod = p_pointer; }
  void setAmountModPointer(float *p_pointer) { m_amount_mod = p_pointer; }
  void setDryWetModPointer(float *p_pointer) { m_drywet_mod = p_pointer; }
  void setFeedbackModPointer(float *p_pointer) { m_res_mod = p_pointer; }

  void setFreqBPM(float p_BPM) { setLFOFreq(p_BPM / m_synctime_ratio / 240.f); }

  void setSynctimeNumerator(float p_value) {
    m_synctime_numerator = p_value;
    m_synctime_ratio = p_value / m_synctime_denominator;
  }

  void setSynctimeDenominator(float p_value) {
    m_synctime_denominator = p_value;
    m_synctime_ratio = m_synctime_numerator / p_value;
  }

  void setLFOResetPos(float p_pos){
    m_LFO_reset_pos = p_pos;
    resetLFO();
  }
  
protected:
  float m_synctime_numerator = 3.f;
  float m_synctime_denominator = 16.f;
  float m_synctime_ratio = 3.f / 16.f;

  float *m_freq_mod;
  float *m_amount_mod;
  float *m_drywet_mod;
  float *m_feedback_mod;

  bool m_LFO_freq_set = false;

  inline void incrementLFO() {
    float increment_modded = m_increment_sine;
    if (*m_freq_mod) {
      //factor 1 / 16 to 16
      increment_modded *= pow(4, *m_freq_mod * 2);
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


  int m_LFO_sign = 1;

  float m_base_time = 0.0105;
  float m_LFO_freq = 0.2;
  float m_LFO_amount = 0.3f;

  float m_index_sine = 0;
  float m_LFO_reset_pos = 0;
  float m_increment_sine;


  float m_dry_wet = 1.f;
};
