#pragma once

#include "../../GlobalIncludes.h"
#include "Filter.h"
#include "VAOnePoleFilter.h"

class LadderFilter : public Filter {
public:
  enum class FILTERTYPE {
    LP4 = 0,
    LP2 = 1,
    BP4 = 2,
    BP2 = 3,
    HP4 = 4,
    HP2 = 5
  };

  LadderFilter(void);
  ~LadderFilter(void);

  // -- Filter Overrides --
  void reset() override;
  void setResControl(double p_res) override;
  void setSampleRate(double p_sr) override {
    Filter::setSampleRate(p_sr);
    m_LPF1.setSampleRate(p_sr);
    m_LPF2.setSampleRate(p_sr);
    m_LPF3.setSampleRate(p_sr);
    m_LPF4.setSampleRate(p_sr);
    m_last_freq_modded = -1; //to signal recalculation of coeffs in update()
  }

  inline void update() {

    // do any modulation first
    Filter::update();

    //! only recalc filter-coefficients if:
    // freq changed
    // res modded
    // res changed (m_last_freq_modded wil be set to -1)
    // sample rate changed (set to -1)
    // filter type was changed (set to -1)

    if(m_last_freq_modded == m_freq_modded && !(*m_res_mod) ){
      return;
    }
    m_last_freq_modded = m_freq_modded;


    m_k_modded = m_k + 4 * (*m_res_mod);
    m_k_modded = m_k_modded > 3.88 ? 3.88 : m_k_modded;
    m_k_modded = m_k_modded < 0 ? 0 : m_k_modded;

    // prewarp for BZT
    double wd = 2 * M_PI * m_freq_modded;
    //double T = 1 / m_samplerate;

    //note: measured input to tan function, it seemed limited to (0.005699, 1.282283). 
    //input for fasttan shall be limited to (-pi/2, pi/2) according to documentation
    double wa = (2 * m_samplerate) * juce::dsp::FastMathApproximations::tan(wd * m_one_over_samplerate * 0.5);
    double g = wa * m_one_over_samplerate * 0.5;

    // G - the feedforward coeff in the VA One Pole
    //     same for LPF, HPF
    double G = g / (1.0 + g);

    // set alphas
    m_LPF1.m_alpha = G;
    m_LPF2.m_alpha = G;
    m_LPF3.m_alpha = G;
    m_LPF4.m_alpha = G;

    // set betas
    m_LPF1.m_beta = G * G * G / (1.0 + g);
    m_LPF2.m_beta = G * G / (1.0 + g);
    m_LPF3.m_beta = G / (1.0 + g);
    m_LPF4.m_beta = 1.0 / (1.0 + g);

    m_gamma = G * G * G * G; // G^4

    m_alpha_0 = 1.0 / (1.0 + m_k_modded * m_gamma);

    // Oberheim variation
    switch (m_filter_type) {
    case FILTERTYPE::LP4:
      m_a = 0.0;
      m_b = 0.0;
      m_c = 0.0;
      m_d = 0.0;
      m_e = 1.0;
      break;

    case FILTERTYPE::LP2:
      m_a = 0.0;
      m_b = 0.0;
      m_c = 1.0;
      m_d = 0.0;
      m_e = 0.0;
      break;

    case FILTERTYPE::BP4:
      m_a = 0.0;
      m_b = 0.0;
      m_c = 4.0;
      m_d = -8.0;
      m_e = 4.0;
      break;

    case FILTERTYPE::BP2:
      m_a = 0.0;
      m_b = 2.0;
      m_c = -2.0;
      m_d = 0.0;
      m_e = 0.0;
      break;

    case FILTERTYPE::HP4:
      m_a = 1.0;
      m_b = -4.0;
      m_c = 6.0;
      m_d = -4.0;
      m_e = 1.0;
      break;

    case FILTERTYPE::HP2:
      m_a = 1.0;
      m_b = -2.0;
      m_c = 1.0;
      m_d = 0.0;
      m_e = 0.0;
      break;

    default: // LP4
      m_a = 0.0;
      m_b = 0.0;
      m_c = 0.0;
      m_d = 0.0;
      m_e = 1.0;
      break;
    }
  }

  inline double doFilter(double xn) {
    jassert(m_samplerate > 0);

    double dSigma = m_LPF1.getFeedbackOutput() + m_LPF2.getFeedbackOutput() +
                    m_LPF3.getFeedbackOutput() + m_LPF4.getFeedbackOutput();

    // calculate input to first filter
    double dU = (xn - m_k_modded * dSigma) * m_alpha_0;

    // --- cascade of 4 filters
    double dLP1 = m_LPF1.doFilter(dU);
    double dLP2 = m_LPF2.doFilter(dLP1);
    double dLP3 = m_LPF3.doFilter(dLP2);
    double dLP4 = m_LPF4.doFilter(dLP3);

    // --- Oberheim variations
    double output =
        m_a * dU + m_b * dLP1 + m_c * dLP2 + m_d * dLP3 + m_e * dLP4;

    applyOverdrive(output);

    return output;
  }

  inline void setFilterType(int p_filtertype) {
    m_filter_type = (FILTERTYPE)p_filtertype;
    m_last_freq_modded = -1; //to signal recalculation of coeffs in update()
  }

  VAOnePoleFilter m_LPF1;
  VAOnePoleFilter m_LPF2;
  VAOnePoleFilter m_LPF3;
  VAOnePoleFilter m_LPF4;

  FILTERTYPE m_filter_type = FILTERTYPE::LP4;

  double m_last_freq_modded = -1;

  // variables
  double m_k; // K, set with Q
  double m_k_modded;
  double m_gamma;   // see block diagram
  double m_alpha_0; // see block diagram

  // Oberheim Xpander variations
  double m_a;
  double m_b;
  double m_c;
  double m_d;
  double m_e;
};
