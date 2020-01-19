#pragma once

#include "../Filters/DCBlockingFilter.h"
#include "../Filters/VAOnePoleFilter.h"
#include "../OdinConstants.h"

//#include <memory>
#include <cstring>

#define CIRCULAR_BUFFER_LENGTH (MAX_EXPECTED_SAMPLE_RATE * MAX_DELAY_TIME)
#define MOVING_AVERAGE_SAMPLE_LENGTH 441 // 1/100 s at standard smaple rate
#define DUCKING_SCALE_FACTOR                                                   \
  5 // hardly ever will the average reach 1, so we scale it up

class Delay {
public:
  Delay();
  ~Delay();

  float doDelayLeft(float p_left);
  float doDelayRight(float p_right);

  inline void setDelayTime(float p_delay_time) {
    m_delay_time_control = p_delay_time;
  }

  inline void setFeedback(float p_feedback) { m_feedback = p_feedback; }

  inline void setSampleRate(float p_samplerate) { 
  //DBG("setsamplerate delay");
    m_samplerate = p_samplerate;
    m_DC_blocking_filter_left.setSampleRate(p_samplerate);
    m_DC_blocking_filter_right.setSampleRate(p_samplerate);
    m_highpass_left.setSampleRate(p_samplerate);
    m_highpass_right.setSampleRate(p_samplerate);
  }

  inline void incWriteIndex() {
    if (++m_write_index >= CIRCULAR_BUFFER_LENGTH) {
      m_write_index = 0;
    }
  }

  inline void setHPFreq(float p_freq) {
    m_highpass_freq = p_freq;
    m_highpass_left.m_freq_base = p_freq;
    m_highpass_right.m_freq_base = p_freq;
  }

  void setHPFreqModPointer(float *p_pointer) {
    m_highpass_left.setFreqModPointer(p_pointer);
    m_highpass_right.setFreqModPointer(p_pointer);
  }

  inline float linearInterpolation(float p_low, float p_high,
                                   float p_distance) {
    return (1.f - p_distance) * p_low + p_distance * p_high;
  }

  inline void reset() {
    m_write_index = 0;
    memset(circular_buffer_left, 0, CIRCULAR_BUFFER_LENGTH * sizeof(float));
    memset(circular_buffer_right, 0, CIRCULAR_BUFFER_LENGTH * sizeof(float));

    m_highpass_left.reset();
    m_highpass_right.reset();

    m_delay_time_smooth = m_delay_time_control;

    m_DC_blocking_filter_left.reset();
    m_DC_blocking_filter_right.reset();
  }

  inline void setDry(float p_dry) { m_dry = p_dry; }

  inline void setWet(float p_wet) { m_wet = p_wet; }

  inline void setDucking(float p_ducking) { m_ducking_amount = p_ducking; }

  inline float doAverage(float p_input) {
    // abs inpkut for pos only
    p_input = fabs(p_input);

    // do moving average
    m_total_sum += p_input - m_last_mean;
    m_last_mean = m_total_sum / MOVING_AVERAGE_SAMPLE_LENGTH;
    return m_last_mean * DUCKING_SCALE_FACTOR > 1
               ? 1
               : m_last_mean * DUCKING_SCALE_FACTOR;
  }

  void setTimeModPointer(float *p_pointer) { m_time_mod = p_pointer; }

  void setFeedbackModPointer(float *p_pointer) { m_feedback_mod = p_pointer; }

  void setDryModPointer(float *p_pointer) { m_dry_mod = p_pointer; }

  void setWetModPointer(float *p_pointer) { m_wet_mod = p_pointer; }

  void setFreqBPM(float p_BPM) { setDelayTime(240 * m_synctime_ratio / p_BPM); }
  void setSynctimeNumerator(float p_value) {
    m_synctime_numerator = p_value;
    m_synctime_ratio = p_value / m_synctime_denominator;
  }
  void setSynctimeDenominator(float p_value) {
    m_synctime_denominator = p_value;
    m_synctime_ratio = m_synctime_numerator / p_value;
  }

  void setPingPong(bool p_ping_pong){
    m_ping_pong = p_ping_pong;
  }

protected:
  DCBlockingFilter m_DC_blocking_filter_left;
  DCBlockingFilter m_DC_blocking_filter_right;

  float m_synctime_numerator = 3.f;
  float m_synctime_denominator = 16.f;
  float m_synctime_ratio = 3.f / 16.f;

  float *m_time_mod;
  float *m_feedback_mod;
  float *m_dry_mod;
  float *m_wet_mod;

  // buffer length is 4 seks at 192kHz or ~17s at 44.1Khz...
  float circular_buffer_left[CIRCULAR_BUFFER_LENGTH] = {0};
  float circular_buffer_right[CIRCULAR_BUFFER_LENGTH] = {0};

  VAOnePoleFilter m_highpass_left;
  VAOnePoleFilter m_highpass_right;

  int m_write_index = 0;

  // moving average
  float m_total_sum = 0.f;
  float m_last_mean = 0.f;

  // need to be init by synth
  float m_delay_time_control = 1.f;
  float m_delay_time_smooth = 1.f;
  float m_feedback = 0.6;
  float m_samplerate = -1;
  float m_dry = 1.f;
  float m_wet = 0.7f;
  float m_highpass_freq = 80.f;
  float m_ducking_amount = 0.f;

  bool m_ping_pong = false;
};
