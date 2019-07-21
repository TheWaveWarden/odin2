#include "Delay.h"

Delay::Delay() {
  m_highpass_left.setHP();
  m_highpass_right.setHP();
  m_highpass_left.reset();
  m_highpass_right.reset();
  m_highpass_left.m_freq_base = 80.f;
  m_highpass_right.m_freq_base = 80.f;
}

Delay::~Delay() {}

float Delay::doDelayLeft(float p_left) {

  float input_left = p_left;

  //todo something wrong with smoothing, sometimes exactly octave down??
  // smooth only in left delay line
  //if (m_delay_time_smooth > m_delay_time_control) {
    m_delay_time_smooth =
        (m_delay_time_smooth - m_delay_time_control) * 0.99994 +
        m_delay_time_control;
  //} else if (m_delay_time_smooth < m_delay_time_control) {
  //  if (m_delay_time_control - m_delay_time_smooth < 0.5f / m_samplerate) {
  //    m_delay_time_smooth = m_delay_time_control;
  //  } else {
  //    m_delay_time_smooth += 0.5f / m_samplerate;
  //  }
  //}
  float delay_time_modded = m_delay_time_smooth;
  if (*m_time_mod) {
    delay_time_modded *= pow(3, *m_time_mod);
  }

  m_highpass_left.update(); // needs to be done only for modding later

  // calc read index
  float read_index = (float)m_write_index - delay_time_modded * m_samplerate;
  int read_index_trunc = (int)read_index;
  int read_index_next = read_index_trunc + 1;
  float frac = read_index - (float)read_index_trunc;

  // check boundaries
  while (read_index_trunc < 0) {
    read_index_trunc += CIRCULAR_BUFFER_LENGTH;
  }
  while (read_index_next < 0) {
    read_index_next += CIRCULAR_BUFFER_LENGTH;
  }

  p_left = linearInterpolation(circular_buffer_left[read_index_trunc],
                               circular_buffer_left[read_index_next], frac);

  float feedback_modded = m_feedback + *m_feedback_mod;
  feedback_modded = feedback_modded > 1 ? 1 : feedback_modded;
  feedback_modded = feedback_modded < 0 ? 0 : feedback_modded;

  if (m_ping_pong) {
    circular_buffer_right[m_write_index] =
        input_left + p_left * feedback_modded;
  } else {
    circular_buffer_left[m_write_index] = input_left + p_left * feedback_modded;
  }

  // set sample behind readindex to zero to avoid reading that signal when
  // increasing delay time
  // TODO this is bullshit? when increasing bigger than a sample we get jumps?
  circular_buffer_left[read_index_trunc] = 0.f;

  // apply HP filter
  p_left = m_highpass_left.doFilter(p_left);

  // apply ducking
  p_left *= (1.f - doAverage(input_left) * m_ducking_amount);

  float wet_modded = m_wet + *m_wet_mod;
  wet_modded = wet_modded > 1 ? 1 : wet_modded;
  wet_modded = wet_modded < 0 ? 0 : wet_modded;

  float dry_modded = m_dry + *m_dry_mod;
  dry_modded = dry_modded > 1 ? 1 : dry_modded;
  dry_modded = dry_modded < 0 ? 0 : dry_modded;

  p_left = m_DC_blocking_filter_left.doFilter(p_left);

  // return output * wet_modded + p_input * dry_modded;
  return p_left * wet_modded + input_left * dry_modded;
}

float Delay::doDelayRight(float p_right) {

  float input_right = p_right;

  float delay_time_modded = m_delay_time_smooth;
  if (*m_time_mod) {
    delay_time_modded *= pow(3, *m_time_mod);
  }

  m_highpass_right.update(); // needs to be done only for modding later

  // calc read index
  float read_index = (float)m_write_index - delay_time_modded * m_samplerate;
  int read_index_trunc = (int)read_index;
  int read_index_next = read_index_trunc + 1;
  float frac = read_index - (float)read_index_trunc;

  // check boundaries
  while (read_index_trunc < 0) {
    read_index_trunc += CIRCULAR_BUFFER_LENGTH;
  }
  while (read_index_next < 0) {
    read_index_next += CIRCULAR_BUFFER_LENGTH;
  }

  p_right = linearInterpolation(circular_buffer_right[read_index_trunc],
                                circular_buffer_right[read_index_next], frac);

  float feedback_modded = m_feedback + *m_feedback_mod;
  feedback_modded = feedback_modded > 1 ? 1 : feedback_modded;
  feedback_modded = feedback_modded < 0 ? 0 : feedback_modded;

  if (m_ping_pong) {
    circular_buffer_left[m_write_index] =
        input_right + p_right * feedback_modded;
  } else {
    circular_buffer_right[m_write_index] =
        input_right + p_right * feedback_modded;
  }

  // inc only in right delay line
  incWriteIndex();

  // set sample behind readindex to zero to avoid reading that signal when
  // increasing delay time
  // TODO this is bullshit? when increasing bigger than a sample we get jumps?
  circular_buffer_right[read_index_trunc] = 0.f;

  // apply HP filter
  p_right = m_highpass_right.doFilter(p_right);

  // apply ducking
  p_right *= (1.f - doAverage(input_right) * m_ducking_amount);

  float wet_modded = m_wet + *m_wet_mod;
  wet_modded = wet_modded > 1 ? 1 : wet_modded;
  wet_modded = wet_modded < 0 ? 0 : wet_modded;

  float dry_modded = m_dry + *m_dry_mod;
  dry_modded = dry_modded > 1 ? 1 : dry_modded;
  dry_modded = dry_modded < 0 ? 0 : dry_modded;

  p_right = m_DC_blocking_filter_right.doFilter(p_right);

  // return output * wet_modded + p_input * dry_modded;
  return p_right * wet_modded + input_right * dry_modded;
}
