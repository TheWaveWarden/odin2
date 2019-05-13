#include "FormantFilter.h"

FormantFilter::FormantFilter() {
  // m_BPF1.m_filter_type = LadderFilter::FILTERTYPE::LP2;//todo?
  // m_BPF2.m_filter_type = LadderFilter::FILTERTYPE::BP2;

  // m_BPF1.setResControl(0.8);
  // m_BPF2.setResControl(0.8);

  // m_BPF1.enablePassBandCompensation();
  // m_BPF2.enablePassBandCompensation();

  reset();

  updateParabolas();

  setFreqModPointer(&m_zero_dummy);
  // m_BPF1.setFreqModPointer(&m_zero_dummy);
  // m_BPF2.setFreqModPointer(&m_zero_dummy);
  // m_BPF1.setResModPointer(&m_zero_dummy);
  // m_BPF2.setResModPointer(&m_zero_dummy);
  // m_BPF1.setVolModPointer(&m_zero_dummy);
  // m_BPF2.setVolModPointer(&m_zero_dummy);
}

FormantFilter::~FormantFilter() {}

void FormantFilter::reset() {
  // m_BPF1.reset();
  // m_BPF2.reset();
}

void FormantFilter::update() {
  Filter::update();

  float transition_modded = m_transition + *m_transition_mod;
  transition_modded = transition_modded > 1 ? 1 : transition_modded;
  transition_modded = transition_modded < 0 ? 0 : transition_modded;

  m_resonator1.setFrequency(m_a0 * transition_modded * transition_modded +
                            m_b0 * transition_modded + m_c0);
  m_resonator2.setFrequency(m_a1 * transition_modded * transition_modded +
                            m_b1 * transition_modded + m_c1);

  m_volume_scalar = m_a2 * transition_modded * transition_modded +
                    m_b2 * transition_modded + m_c2;

  // m_BPF1.m_freq_base = m_a0 * m_transition * m_transition + m_b0 *
  // m_transition + m_c0; m_BPF2.m_freq_base = m_a1 * m_transition *
  // m_transition
  // + m_b1 * m_transition + m_c1; for(float transition = 0.f; transition
  // <= 1.f; transition += 0.01){
  //  DBG(m_a0 * transition * transition + m_b0 * transition + m_c0);
  //}
  // DBG("==================================================================");

  // m_BPF1.m_freq_base = m_formant_list[m_vowel_left][0] * (1.f - m_transition)
  // + m_transition * m_formant_list[m_vowel_right][0]; m_BPF2.m_freq_base =
  // m_formant_list[m_vowel_left][1] * (1.f - m_transition) + m_transition *
  // m_formant_list[m_vowel_right][1];

  //    m_BPF1.m_mod_frequency = m_formant_width;
  //    m_BPF2.m_mod_frequency = -m_formant_width;

  // m_BPF1.update();
  // m_BPF2.update();
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

// void FormantFilter::setResControl(double p_res){
//    m_BPF1.setResControl(0.3 * p_res + 0.4);
//    m_BPF2.setResControl(0.3 * p_res + 0.4);
//}

void FormantFilter::setTransition(float p_trans) { m_transition = p_trans; }

double FormantFilter::doFilter(double p_input) {
  // double out = m_BPF1.doFilter(m_BPF2.doFilter(p_input));
  double out = m_resonator1.doFilter(m_resonator2.doFilter(p_input));

  float vol_mod_factor =
      (*m_vol_mod) > 0 ? 1.f + 4 * (*m_vol_mod) : (1.f + *m_vol_mod);

  return out * vol_mod_factor * m_volume_scalar;
}