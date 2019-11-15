#pragma once

#include "../../GlobalIncludes.h"
#include "BiquadFilter.h"

#include <cmath>

#define RESONATOR_GAIN_FACTOR 0.08

class BiquadResonator : public BiquadFilter {
public:
  BiquadResonator() {
    //m_b0 = 0.005;
    m_b0 = RESONATOR_GAIN_FACTOR;
    m_b1 = 0;
    //this factor implements additional zeros at 0 and nyquist to normalize peak gain as taken from
    //https://ccrma.stanford.edu/~jos/filters/Time_Varying_Two_Pole_Filters.html
    m_b2 = -RESONATOR_GAIN_FACTOR;
  }
  ~BiquadResonator() {}

  void setRadius(float p_radius) {
    m_radius = p_radius;
    recalculateFilterCoefficients();
  }
  void setSampleRate(float p_sr) { 
    //DBG("setsamplerate biquadres");
    m_samplerate = p_sr;
	  m_one_over_samplerate = 1. / p_sr;
    recalculateFilterCoefficients();
  }
  void setFrequency(float p_freq) {
    m_freq = p_freq;
    recalculateFilterCoefficients();
  }

  void recalculateFilterCoefficients() {
    m_a1 = -2 * m_radius * juce::dsp::FastMathApproximations::cos(2 * M_PI * m_freq * m_one_over_samplerate);
    m_a2 = m_radius * m_radius;
  }

  float min(float a, float b) { return a < b ? a : b; }

protected:

  float m_radius = 0.996;
  float m_freq = 2000;
  float m_samplerate;
  float m_one_over_samplerate;
};
