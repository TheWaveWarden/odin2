#include "SEMFilter12.h"

SEMFilter12::SEMFilter12() {
  m_alpha_0 = 1.0;
  m_alpha = 1.0;
  m_rho = 1.0;
  m_aux_control = 0.5;

  reset();

  m_transition = -1.;
  m_resonance = 0.5f;
  m_resonance_modded = m_resonance;
}

SEMFilter12::~SEMFilter12() {}

void SEMFilter12::reset() {
  m_z_1 = 0.0;
  m_z_2 = 0.0;
}

void SEMFilter12::update() {
  Filter::update();

  double wd = 2 * M_PI * m_freq_modded;
  double T = 1.0 / m_samplerate;
  double wa = (2 / T) * tan(wd * T / 2);
  double g = wa * T / 2;

  m_resonance_modded = m_resonance + (*m_res_mod) * 24.5;
  m_resonance_modded = m_resonance_modded > 25 ? 25 : m_resonance_modded;
  m_resonance_modded = m_resonance_modded < 0.5 ? 0.5 : m_resonance_modded;

  double r = 1.0 / (2.0 * m_resonance_modded);

  m_alpha_0 = 1.0 / (1.0 + 2.0 * r * g + g * g);
  m_alpha = g;
  m_rho = 2.0 * r + g;
}

double SEMFilter12::doFilter(double xn) {
  double hpf = m_alpha_0 * (xn - m_rho * m_z_1 - m_z_2);
  double bpf = m_alpha * hpf + m_z_1;

  double lpf = m_alpha * bpf + m_z_2;
  double r = 1.0 / (2.0 * m_resonance_modded);
  double bsf = xn - 2.0 * r * bpf;

  m_z_1 = m_alpha * hpf + bpf;
  m_z_2 = m_alpha * bpf + lpf;

  float transition_modded = m_transition + *m_transition_mod * 2;
  transition_modded = transition_modded > 1 ? 1 : transition_modded;
  transition_modded = transition_modded < -1 ? -1 : transition_modded;

  if (transition_modded < 0) {
    xn = (1 + transition_modded) * bsf - transition_modded * lpf;
  } else {
    xn = transition_modded * hpf + (1 - transition_modded) * bsf;
  }

  float vol_mod_factor =
      (*m_vol_mod) > 0 ? 1.f + 4 * (*m_vol_mod) : (1.f + *m_vol_mod);
  vol_mod_factor = vol_mod_factor > VOL_MOD_UPPER_LIMIT ? VOL_MOD_UPPER_LIMIT
                                                        : vol_mod_factor;

  return xn * vol_mod_factor;
}

void SEMFilter12::setResControl(double p_res) {
  m_resonance = 24.5 * p_res * p_res * p_res * p_res + 0.5;
}
