#include "ChiptuneArpeggiator.h"
#include <cmath>


float pitchShiftMultiplier(float p_semitones) {
  return pow(2.f, p_semitones / 12.f);
}

int ChiptuneArpeggiator::doArpeggiator() {
  if (!m_arp_active) {
    return 0;
  }

  if (*m_freq_mod != 0) {
    m_inc = m_frequency / m_samplerate * pitchShiftMultiplier(*m_freq_mod * 24);
  } else {
    m_inc = m_frequency / m_samplerate;
  }

  // inc & wrap position inside step
  m_position_in_step += m_inc;
  while (m_position_in_step > STEP_LENGTH) {
    m_position_in_step -= STEP_LENGTH;
    ++m_current_step;
    // inc & wrap step
    if (m_current_step > 2 || (!m_step_three_active && m_current_step > 1)) {
      m_current_step = 0;
    }
  }

  return m_step_semitones[m_current_step];
}
