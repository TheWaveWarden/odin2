#include "Delay.h"

Delay::Delay() {
  m_highpass.setHP();
  m_highpass.reset();
  m_highpass.m_freq_base = 80.f;
}

Delay::~Delay() {}

float Delay::doDelay(float p_input) {

  // TODO implement sync mode
  // todo audio bug when increasing delay_time
  if (m_delay_time_smooth > m_delay_time_control) {
    m_delay_time_smooth =
        (m_delay_time_smooth - m_delay_time_control) * 0.99994 +
        m_delay_time_control;
  } else if (m_delay_time_smooth < m_delay_time_control) {
    if (m_delay_time_control - m_delay_time_smooth < 0.5f / m_samplerate) {
      m_delay_time_smooth = m_delay_time_control;
    } else {
      m_delay_time_smooth += 0.5f / m_samplerate;
    }
  }
  float delay_time_modded = m_delay_time_smooth;
  if (*m_time_mod) {
    delay_time_modded *= pow(3, *m_time_mod);
  }

  m_highpass.update(); // needs to be done only for modding later

  // calc read index
  float read_index = (float)m_write_index - delay_time_modded * m_samplerate;
  int read_index_trunc = (int)read_index;
  int read_index_next = read_index_trunc + 1;
  float frac = read_index - (float)read_index_trunc;

  //   read_index_trunc =
  //                          read_index_trunc + CIRCULAR_BUFFER_LENGTH
  //                          : read_index_trunc;
  //   read_index_next = read_index_next < 0
  //                         read_index_next + CIRCULAR_BUFFER_LENGTH
  //                          read_index_next;

  // check boundaries
  while (read_index_trunc < 0) {
    read_index_trunc += CIRCULAR_BUFFER_LENGTH;
  }
  while (read_index_next < 0) {
    read_index_next += CIRCULAR_BUFFER_LENGTH;
  }

  float output = linearInterpolation(circular_buffer[read_index_trunc],
                                     circular_buffer[read_index_next], frac);

  float feedback_modded = m_feedback + *m_feedback_mod;
  feedback_modded = feedback_modded > 1 ? 1 : feedback_modded;
  feedback_modded = feedback_modded < 0 ? 0 : feedback_modded;

  circular_buffer[m_write_index] = p_input + output * feedback_modded;
  incWriteIndex();

  // set sample behind readindex to zero to avoid reading that signal when
  // increasing delay time
  circular_buffer[read_index_trunc] = 0.f;

  // apply HP filter
  output = m_highpass.doFilter(output);

  // apply ducking
  output *= (1.f - doAverage(p_input) * m_ducking_amount);

  float wet_modded = m_wet + *m_wet_mod;
  wet_modded = wet_modded > 1 ? 1 : wet_modded;
  wet_modded = wet_modded < 0 ? 0 : wet_modded;

  float dry_modded = m_dry + *m_dry_mod;
  dry_modded = dry_modded > 1 ? 1 : dry_modded;
  dry_modded = dry_modded < 0 ? 0 : dry_modded;

  output = m_DC_blocking_filter.doFilter(output);
  return output * wet_modded + p_input * dry_modded;
}
