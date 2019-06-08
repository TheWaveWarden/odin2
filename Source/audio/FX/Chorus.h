#pragma once

#include <cmath>
#include <cstring>

#define CHORUS_BUFFER_LENGTH 44100
#define CHORUS_MIN_DISTANCE                                                    \
  2 // min distance in samples the read index is behind the write index
#define CHORUS_AMOUNT_RANGE 0.035 // empiric value

class Chorus {
public:
  Chorus();
  ~Chorus();

  float doChorus(float p_input);

  inline void setSamplerate(float p_samplerate) {
    // store LFO freq
    float LFO_freq = m_LFO_inc / 2.f / m_samplerate;
    m_samplerate = p_samplerate;
    if (!m_LFO_freq_set) {
      setLFOFreq(LFO_freq);
      m_LFO_freq_set = true;
    } else {
      setLFOFreq(0.2f); // this is initial
    }
  }

  inline void setAmount(float p_amount) { m_amount = p_amount * p_amount; }

  inline void setDryWet(float p_dry_wet) { m_dry_wet = p_dry_wet; }

  inline void setLFOFreq(float p_LFO_freq) {
    m_LFO_inc = 2 * p_LFO_freq / m_samplerate;
  }

  inline void reset() {
    memset(m_circular_buffer, 0, CHORUS_BUFFER_LENGTH * sizeof(float));
    m_LFO_pos = 0;
  }

  inline void incLFO() {
    float increment_modded = m_LFO_inc;
    if (*m_freq_mod) {
      increment_modded *= pow(4, *m_freq_mod);
    }
    m_LFO_pos += increment_modded;
    while (m_LFO_pos > 2.f) {
      m_LFO_pos -= 2.f;
    }
  }

  inline void incWriteIndex() {
    if (++m_write_index >= CHORUS_BUFFER_LENGTH) {
      m_write_index = 0;
    }
  }

  inline float linearInterpolation(float p_low, float p_high,
                                   float p_distance) {
    return (1.f - p_distance) * p_low + p_distance * p_high;
  }

  // calc cheap sine and cosine (2-periodic)
  inline void doLFO(float &pi_LFO1, float &pi_LFO2) {
    float LFO2_pos = m_LFO_pos + 0.5f;
    LFO2_pos = LFO2_pos > 2.f ? LFO2_pos - 2.f : LFO2_pos;

    int LFO_sign = m_LFO_pos < 1 ? 1 : -1;
    float calc_pos = m_LFO_pos > 1 ? m_LFO_pos - 1 : m_LFO_pos;
    pi_LFO1 = 4.f * (calc_pos * (1.f - calc_pos)) * LFO_sign;

    LFO_sign = LFO2_pos < 1 ? 1 : -1;
    calc_pos = LFO2_pos > 1 ? LFO2_pos - 1 : LFO2_pos;
    pi_LFO2 = 4.f * (calc_pos * (1.f - calc_pos)) * LFO_sign;
  }
  void setFreqModPointer(float *p_pointer) { m_freq_mod = p_pointer; }
  void setAmountModPointer(float *p_pointer) { m_amount_mod = p_pointer; }
  void setDryWetModPointer(float *p_pointer) { m_drywet_mod = p_pointer; }

protected:
  float *m_freq_mod;
  float *m_amount_mod;
  float *m_drywet_mod;

  float m_circular_buffer[CHORUS_BUFFER_LENGTH] = {0};
  float m_samplerate;
  float m_dry_wet = 1.f;
  float m_LFO_inc;
  float m_LFO_pos = 0;
  float m_amount = 0.3f;
  int m_write_index = 0;

  bool m_LFO_freq_set = false;
};
