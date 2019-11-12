#include "Korg35Filter.h"

Korg35Filter::Korg35Filter(void) {
  m_is_lowpass = true;

  // init
  m_k = 0.01;
  m_alpha = 0.0;

  m_LPF1.setLP();
  m_LPF2.setLP();
  m_HPF1.setHP();
  m_HPF2.setHP();

  reset();
}

Korg35Filter::~Korg35Filter(void) {}

void Korg35Filter::reset() {
  m_LPF1.reset();
  m_LPF2.reset();
  m_HPF1.reset();
  m_HPF2.reset();
}

void Korg35Filter::update() {
  Filter::update();

  // BZT
  double wd = 2 * 3.141592653 * m_freq_modded;
  double t = 1.0 / m_samplerate;
  double wa = (2.0 / t) * juce::dsp::FastMathApproximations::tan(wd * t / 2.0);
  double g = wa * t / 2.0;

  double G = g / (1.0 + g);

  m_LPF1.m_alpha = G;
  m_LPF2.m_alpha = G;
  m_HPF1.m_alpha = G;
  m_HPF2.m_alpha = G;

  m_k_modded = m_k + (*m_res_mod) * 2;
  m_k_modded = m_k_modded > 1.96 ? 1.96 : m_k_modded;
  m_k_modded = m_k_modded < 0.01 ? 0.01 : m_k_modded;

  m_alpha = 1.0 / (1.0 - m_k_modded * G + m_k_modded * G * G);

  if (m_is_lowpass) {
    m_LPF2.m_beta = (m_k_modded - m_k_modded * G) / (1.0 + g);
    m_HPF1.m_beta = -1.0 / (1.0 + g);
    m_LPF1.m_beta = (m_k_modded - m_k_modded * G) / (1.0 + g);
    m_HPF2.m_beta = -1.0 / (1.0 + g);
  } else {
    m_HPF2.m_beta = -1.0 * G / (1.0 + g);
    m_LPF1.m_beta = 1.0 / (1.0 + g);
    m_HPF2.m_beta = -1.0 * G / (1.0 + g);
    m_LPF1.m_beta = 1.0 / (1.0 + g);
  }
}

double Korg35Filter::doFilter(double xn) {
  double y;
  if (m_is_lowpass) {
    double y1 = m_LPF1.doFilter(xn);
    double s35 = m_LPF2.getFeedbackOutput() + m_HPF1.getFeedbackOutput();
    double u = m_alpha * (y1 + s35);

    y = m_k_modded * m_LPF2.doFilter(u);
    m_HPF1.doFilter(y);
  } else {
    double y1 = m_HPF1.doFilter(xn);
    double s35 = m_HPF2.getFeedbackOutput() + m_LPF1.getFeedbackOutput();
    double u = m_alpha * (y1 + s35);

    y = m_k_modded * u;
    m_LPF1.doFilter(m_HPF2.doFilter(y));
  }
  y /= m_k_modded;

  // make this one a bit easier (3.f), its very aggresive
  applyOverdrive(y, 3.f);

  float vol_mod_factor =
      (*m_vol_mod) > 0 ? 1.f + 4 * (*m_vol_mod) : (1.f + *m_vol_mod);
  vol_mod_factor = vol_mod_factor > VOL_MOD_UPPER_LIMIT ? VOL_MOD_UPPER_LIMIT
                                                        : vol_mod_factor;

  return y * vol_mod_factor;
}

void Korg35Filter::setResControl(double res) {
  // note: m_k must never be zero else division by zero
  // note2 original was 1.99 but dont want self oscillation
  m_k = res * 1.95 + 0.01;
}
