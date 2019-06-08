#pragma once

#include "../OdinConstants.h"
#include "Filter.h"
#include <cstring>

#define COMB_BUFFER_LENGTH (MAX_EXPECTED_SAMPLE_RATE / FILTER_FC_MIN)

//#include "../JuceLibraryCode/JuceHeader.h"//todo remove

class CombFilter {
public:
  CombFilter();
  ~CombFilter();

  float doFilter(float p_input);
  int m_MIDI_note = 0;
  int m_MIDI_velocity = 0.f;

  float m_kbd_mod_amount = 0;
  float m_vel_mod_amount = 0;
  float m_env_mod_amount = 0;
  float m_env_value = 0;


  float pitchShiftMultiplier(float p_semitones) {
    return pow(2.f, p_semitones / 12.f);
  }

  void setFreqModPointer(float *p_pointer) { m_freq_mod = p_pointer; }
  void setVolModPointer(float *p_pointer) { m_vol_mod = p_pointer; }
  void setEnvModPointer(float *p_pointer) { m_env_mod_mod = p_pointer; }
  void setVelModPointer(float *p_pointer) { m_vel_mod_mod = p_pointer; }
  void setKbdModPointer(float *p_pointer) { m_kbd_mod_mod = p_pointer; }
  

  inline void setPositive(bool p_positive) {
    m_positive_comb = p_positive ? 1 : -1;
  }

  inline void setCombFreq(float p_freq) {
    // TODO check for too long time... here? no bc gets modded later
    m_delay_time_control = 1.f / p_freq;
  }

  inline void setResonance(float p_feedback) { m_feedback = p_feedback; }

  inline virtual void setSamplerate(float p_samplerate) { m_samplerate = p_samplerate; }

  inline void incWriteIndex() {
    if (++m_write_index >= COMB_BUFFER_LENGTH) {
      m_write_index = 0;
    }
  }

  inline float linearInterpolation(float p_low, float p_high,
                                   float p_distance) {
    return (1.f - p_distance) * p_low + p_distance * p_high;
  }

  inline virtual void reset() {
    m_write_index = 0;
    memset(circular_buffer, 0, COMB_BUFFER_LENGTH * sizeof(float));
  }

  void setResModPointer(float* p_pointer){
    m_res_mod = p_pointer;
  }

  float* m_freq_mod = &m_zero_modulation_dummy;
  float* m_res_mod = &m_zero_modulation_dummy;
  float* m_vol_mod = &m_zero_modulation_dummy;
  float* m_env_mod_mod = &m_zero_modulation_dummy;
  float* m_vel_mod_mod = &m_zero_modulation_dummy;
  float* m_kbd_mod_mod = &m_zero_modulation_dummy;

  float m_zero_modulation_dummy = 0.f;
  float m_feedback_lower_limit = 0.f;//used for resonance modulation down (is -1 in flanger)
  float m_feedback_higher_limit = 1.f;//used for resonance modulation down (is 0.97 in flanger)
  // buffer length is 4 seks at 192kHz or ~17s at 44.1Khz...
  float circular_buffer[COMB_BUFFER_LENGTH] = {0};

  int m_write_index = 0;
  int m_positive_comb = 1;

  // need to be init by synth
  double m_delay_time_control = 1. / 2000.f;
  double m_delay_time_smooth = 1. / 2000.f;
  float m_feedback = 0;
  float m_samplerate = 44100;
};
