#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

#include "../Filters/BiquadAllpass.h"
#include <cmath>

#define PHASER_MAX_LFO_AMPLITUDE 4000

class Phaser {
public:
  Phaser();
  ~Phaser();

  inline void setSampleRate(float p_samplerate) {

    //DBG("setsamplerate phaser");
    m_samplerate = p_samplerate;
    setLFOFreq(m_LFO_freq);

    m_AP1_left.setSampleRate(p_samplerate);
    m_AP2_left.setSampleRate(p_samplerate);
    m_AP3_left.setSampleRate(p_samplerate);
    m_AP4_left.setSampleRate(p_samplerate);
    m_AP5_left.setSampleRate(p_samplerate);
    m_AP6_left.setSampleRate(p_samplerate);
    m_AP7_left.setSampleRate(p_samplerate);
    m_AP8_left.setSampleRate(p_samplerate);
    m_AP9_left.setSampleRate(p_samplerate);
    m_AP10_left.setSampleRate(p_samplerate);
    m_AP11_left.setSampleRate(p_samplerate);
    m_AP12_left.setSampleRate(p_samplerate);

    m_AP1_right.setSampleRate(p_samplerate);
    m_AP2_right.setSampleRate(p_samplerate);
    m_AP3_right.setSampleRate(p_samplerate);
    m_AP4_right.setSampleRate(p_samplerate);
    m_AP5_right.setSampleRate(p_samplerate);
    m_AP6_right.setSampleRate(p_samplerate);
    m_AP7_right.setSampleRate(p_samplerate);
    m_AP8_right.setSampleRate(p_samplerate);
    m_AP9_right.setSampleRate(p_samplerate);
    m_AP10_right.setSampleRate(p_samplerate);
    m_AP11_right.setSampleRate(p_samplerate);
    m_AP12_right.setSampleRate(p_samplerate);
  }

  inline void reset() {
    m_AP1_left.reset();
    m_AP2_left.reset();
    m_AP3_left.reset();
    m_AP4_left.reset();
    m_AP5_left.reset();
    m_AP6_left.reset();
    m_AP7_left.reset();
    m_AP8_left.reset();
    m_AP9_left.reset();
    m_AP10_left.reset();
    m_AP11_left.reset();
    m_AP12_left.reset();

    m_AP1_right.reset();
    m_AP2_right.reset();
    m_AP3_right.reset();
    m_AP4_right.reset();
    m_AP5_right.reset();
    m_AP6_right.reset();
    m_AP7_right.reset();
    m_AP8_right.reset();
    m_AP9_right.reset();
    m_AP10_right.reset();
    m_AP11_right.reset();
    m_AP12_right.reset();
    m_index_sine_left = 0;
    m_index_sine_right = 0;
    m_store_output_left = 0;
    m_store_output_right = 0;
  }

  float doPhaserLeft(float p_input);
  float doPhaserRight(float p_input);

  void setDryWet(float p_amount);

  inline void setRadiusBase(float p_radius_base) {
    m_radius_base = p_radius_base; // is initialized to 1.25
    setRadius(m_radius_base);
  }

  inline void setLFOFreq(float p_freq) {
    m_increment_sine = 2 * p_freq / m_samplerate;
    m_LFO_freq = p_freq;
  }

  inline void setLFOAmplitude(float p_amplitude) { m_amount = p_amplitude; }

  // make this one a stereo phaser...????

  inline void setBaseFreq(float p_base_freq) { m_base_freq = p_base_freq; }

  // void setWidth(float p_width){m_width = p_width * 2.5f;}

  void setRateModPointer(float *p_pointer) { m_rate_mod = p_pointer; }
  void setAmountModPointer(float *p_pointer) { m_amount_mod = p_pointer; }
  void setDryWetModPointer(float *p_pointer) { m_drywet_mod = p_pointer; }
  void setFreqModPointer(float *p_pointer) { m_freq_mod = p_pointer; }
  void setFeedbackModPointer(float *p_pointer) { m_feedback_mod = p_pointer; }

  void setFeedback(float p_feedback) { m_feedback = p_feedback * 0.97; }
  void resetLFO() {
    m_index_sine_left = 0;
    m_index_sine_right = 0.5;
  }
  void setFreqBPM(float p_BPM) { setLFOFreq(p_BPM / m_synctime_ratio / 240.f); }
  void setSynctimeNumerator(float p_value) {
    m_synctime_numerator = p_value;
    m_synctime_ratio = p_value / m_synctime_denominator;
  }
  void setSynctimeDenominator(float p_value) {
    m_synctime_denominator = p_value;
    m_synctime_ratio = m_synctime_numerator / p_value;
  }

protected:
  float m_synctime_numerator = 3.f;
  float m_synctime_denominator = 16.f;
  float m_synctime_ratio = 3.f / 16.f;

  float *m_rate_mod;
  float *m_drywet_mod;
  float *m_amount_mod;
  float *m_freq_mod;
  float *m_feedback_mod;

  float m_width = 1.f;

  bool m_LFO_freq_set = false;

  inline void incrementLFOLeft() {
    float increment_modded = m_increment_sine;
    if (*m_rate_mod) {
      increment_modded *= pow(4, *m_rate_mod);
    }
    m_index_sine_left += increment_modded;
    while (m_index_sine_left > 1) {
      m_index_sine_left -= 1.f;
      m_LFO_sign_left *= -1;
    }
  }

  inline void incrementLFORight() {
    float increment_modded = m_increment_sine;
    if (*m_rate_mod) {
      //factor 1/16 to 16
      increment_modded *= pow(4, *m_rate_mod * 2);
    }
    m_index_sine_right += increment_modded;
    while (m_index_sine_right > 1) {
      m_index_sine_right -= 1.f;
      m_LFO_sign_right *= -1;
    }
  }

  inline float doLFOLeft() {
    // cheap approximation by parabola
    return 4 * (m_index_sine_left * (1 - m_index_sine_left)) * m_LFO_sign_left;
  }
  inline float doLFORight() {
    // cheap approximation by parabola
    return 4 * (m_index_sine_right * (1 - m_index_sine_right)) *
           m_LFO_sign_right;
  }
  void setRadius(float p_radius);
  void setFrequencyLeft(float p_frequency);
  void setFrequencyRight(float p_frequency);

  float m_drywet = 0.5f;
  float m_base_freq = 4000.f;

  BiquadAllpass m_AP1_left;
  BiquadAllpass m_AP2_left;
  BiquadAllpass m_AP3_left;
  BiquadAllpass m_AP4_left;
  BiquadAllpass m_AP5_left;
  BiquadAllpass m_AP6_left;
  BiquadAllpass m_AP7_left;
  BiquadAllpass m_AP8_left;
  BiquadAllpass m_AP9_left;
  BiquadAllpass m_AP10_left;
  BiquadAllpass m_AP11_left;
  BiquadAllpass m_AP12_left;

  BiquadAllpass m_AP1_right;
  BiquadAllpass m_AP2_right;
  BiquadAllpass m_AP3_right;
  BiquadAllpass m_AP4_right;
  BiquadAllpass m_AP5_right;
  BiquadAllpass m_AP6_right;
  BiquadAllpass m_AP7_right;
  BiquadAllpass m_AP8_right;
  BiquadAllpass m_AP9_right;
  BiquadAllpass m_AP10_right;
  BiquadAllpass m_AP11_right;
  BiquadAllpass m_AP12_right;

  int m_LFO_sign_left =
      1; // switches from the positive to the negative proportion of the LFO
  int m_LFO_sign_right =
      1; // switches from the positive to the negative proportion of the LFO

  float m_LFO_freq = 0.25;

  float m_amount = 0.3f;
  float m_index_sine_left = 0;
  float m_index_sine_right = 0.5; // quarter phase shifted
  float m_increment_sine = 0.25;

  float m_radius_base; // is initialized to 1.25
  float m_samplerate;

  float m_feedback = 0.25f * 0.97f;
  float m_store_output_left = 0;
  float m_store_output_right = 0;
};
