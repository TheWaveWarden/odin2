#pragma once

#include "../../GlobalIncludes.h"
#include "BiquadFilter.h"

#include <cmath>

class BiquadResonator : public BiquadFilter {
public:
  BiquadResonator() {
    m_b0 = 0.005;
    m_b1 = 0;
    m_b2 = 0;
    m_a2 = 1;
  }
  ~BiquadResonator() {}

  double doFilter(double p_input) override {
    return BiquadFilter::doFilter(p_input);// * m_output_scalar;
  }

  void setRadius(float p_radius) {
    m_radius = p_radius;
    recalculateFilterCoefficients();
  }
  void setSampleRate(float p_sr) {
    m_samplerate = p_sr;
    recalculateFilterCoefficients();
  }
  void setFrequency(float p_freq) {
    m_freq = p_freq;
    recalculateFilterCoefficients();
  }

  void recalculateFilterCoefficients() {
    float cosine = cos(2 * M_PI * m_freq / m_samplerate);
    m_a1 = -2 * m_radius * cosine;
    m_a2 = m_radius * m_radius;

    //DBG(1 - 2 * m_radius * cosine + m_a2);
    //DBG(1 + 2 * m_radius * cosine + m_a2);

    m_output_scalar = min(1 - 2 * m_radius * cosine + m_a2,
                              1 + 2 * m_radius * cosine + m_a2);
    //DBG(m_output_scalar);
    //DBG("======");
  }

  float min(float a, float b) { return a < b ? a : b; }

protected:
  float m_output_scalar =
      1.f; // used to scale the output so the spectrum at 0 and nyquist is max 1
  float m_radius = 0.996;
  float m_freq = 2000;
  float m_samplerate = 48000.f;
};
