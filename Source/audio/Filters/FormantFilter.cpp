#include "FormantFilter.h"

#define FORMANT_OUTPUT_SCALAR 0.5f

FormantFilter::FormantFilter() {
  reset();
  updateParabolas();
  setFreqModPointer(&m_zero_dummy);
}

FormantFilter::~FormantFilter() {}

void FormantFilter::reset() {
  m_resonator1.reset();
  m_resonator2.reset();
}

void FormantFilter::update() {

  float vel_modded = m_vel_mod_amount + *m_vel_mod_mod < 0
                         ? 0
                         : m_vel_mod_amount + *m_vel_mod_mod;

  float transition_modded = m_transition + *m_transition_mod +
                            vel_modded * (float)m_MIDI_velocity / 127.f +
                            (m_env_mod_amount + *m_env_mod_mod) * m_env_value;
  transition_modded = transition_modded > 1 ? 1 : transition_modded;
  transition_modded = transition_modded < 0 ? 0 : transition_modded;

  m_resonator1.setFrequency(m_a0 * transition_modded * transition_modded +
                            m_b0 * transition_modded + m_c0);
  m_resonator2.setFrequency(m_a1 * transition_modded * transition_modded +
                            m_b1 * transition_modded + m_c1);

  m_volume_scalar = m_a2 * transition_modded * transition_modded +
                    m_b2 * transition_modded + m_c2;
}

void FormantFilter::updateParabolas() {
  float f0 = m_formant_list[m_vowel_left][0];
  float f1 = m_formant_list[m_vowel_right][0];
  float f2 = f0 * pow(f1 / f0, 0.5);

  m_a0 = 2 * f1 - 4 * f2 + 2 * f0;
  m_b0 = 4 * f2 - 3 * f0 - f1;
  m_c0 = f0;

  f0 = m_formant_list[m_vowel_left][1];
  f1 = m_formant_list[m_vowel_right][1];
  f2 = f0 * pow(f1 / f0, 0.5);

  m_a1 = 2 * f1 - 4 * f2 + 2 * f0;
  m_b1 = 4 * f2 - 3 * f0 - f1;
  m_c1 = f0;

  // last one is for volume
  f0 = m_formant_list[m_vowel_left][2];
  f1 = m_formant_list[m_vowel_right][2];
  f2 = f0 * pow(f1 / f0, 0.5);

  m_a2 = 2 * f1 - 4 * f2 + 2 * f0;
  m_b2 = 4 * f2 - 3 * f0 - f1;
  m_c2 = f0;
}

void FormantFilter::setTransition(float p_trans) { m_transition = p_trans; }

double FormantFilter::doFilter(double p_input) {

  double out = m_resonator1.doFilter(m_resonator2.doFilter(p_input));

  float vol_mod_factor =
      (*m_vol_mod) > 0 ? 1.f + 4 * (*m_vol_mod) : (1.f + *m_vol_mod);

  return out * vol_mod_factor * m_volume_scalar * FORMANT_OUTPUT_SCALAR;
}