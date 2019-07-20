#include "CombFilter.h"

CombFilter::CombFilter() {}

CombFilter::~CombFilter() {}

float CombFilter::doFilter(float p_input) {

  // todo audio bug when increasing delay_time

  m_delay_time_smooth = (m_delay_time_smooth - m_delay_time_control) * 0.999 +
                        m_delay_time_control;

  float delay_time_modded = m_delay_time_smooth;
  if (*m_freq_mod + m_vel_mod_amount + *m_vel_mod_mod + m_env_mod_amount +
          *m_env_mod_mod ||
      m_kbd_mod_amount + *m_kbd_mod_mod) {
    delay_time_modded *= pitchShiftMultiplier(
        ((-*m_freq_mod) - m_env_value * (m_env_mod_amount + *m_env_mod_mod) -
         (m_vel_mod_amount + *m_vel_mod_mod) * (float)m_MIDI_velocity / 127.f) *
            48 -
        m_MIDI_note * (m_kbd_mod_amount + *m_kbd_mod_mod));
  }
  delay_time_modded =
      delay_time_modded > 1.f / 40.f ? 1.f / 40.f : delay_time_modded;

  // calc read index
  float read_index = (float)m_write_index - delay_time_modded * m_samplerate;
  int read_index_trunc = floor(read_index);
  int read_index_next = read_index_trunc + 1;
  float frac = read_index - (float)read_index_trunc;

  // check boundaries
  // while(read_index_trunc < 0){
  //    read_index_trunc += COMB_BUFFER_LENGTH;
  //}
  // while(read_index_next < 0){
  //    read_index_next += COMB_BUFFER_LENGTH;
  //}
  // while(read_index_trunc >= COMB_BUFFER_LENGTH ){
  //   read_index_trunc -= COMB_BUFFER_LENGTH;
  //}
  // while(read_index_next >= COMB_BUFFER_LENGTH ){
  //    read_index_next -= COMB_BUFFER_LENGTH;
  //}
  read_index_trunc = read_index_trunc < 0
                         ? read_index_trunc + COMB_BUFFER_LENGTH
                         : read_index_trunc;
  read_index_next = read_index_next < 0 ? read_index_next + COMB_BUFFER_LENGTH
                                        : read_index_next;
  read_index_next = read_index_next >= COMB_BUFFER_LENGTH
                        ? read_index_next - COMB_BUFFER_LENGTH
                        : read_index_next;

  float output = linearInterpolation(circular_buffer[read_index_trunc],
                                     circular_buffer[read_index_next], frac);

  float feedback_modded = m_feedback + *m_res_mod;
  feedback_modded = feedback_modded > m_feedback_higher_limit
                        ? m_feedback_higher_limit
                        : feedback_modded;
  feedback_modded = feedback_modded < m_feedback_lower_limit
                        ? m_feedback_lower_limit
                        : feedback_modded;

  circular_buffer[m_write_index] =
      p_input + output * feedback_modded * m_positive_comb;
  incWriteIndex();

  // set sample behind readindex to zero to avoid reading that signal when
  // increasing delay time circular_buffer[read_index_trunc] = 0.f; // todo not
  // enough? (time jumps over this)

  float vol_mod_factor =
      (*m_vol_mod) > 0 ? 1.f + 4 * (*m_vol_mod) : (1.f + *m_vol_mod);
  vol_mod_factor = vol_mod_factor > VOL_MOD_UPPER_LIMIT ? VOL_MOD_UPPER_LIMIT
                                                        : vol_mod_factor;

  float ret = (p_input + output) * 0.5f * vol_mod_factor;
  m_DC_blocking_filter.doFilter(ret);
  return ret;
}
