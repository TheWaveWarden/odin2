/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 TheWaveWarden
**
** Odin 2 is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Odin 2 is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/

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
  virtual ~BiquadResonator() {}

  void setRadius(float p_radius) {
    m_radius = p_radius;
    recalculateFilterCoefficients();
  }
  void setSampleRate(float p_sr) { 
    BiquadFilter::setSampleRate(p_sr);
    recalculateFilterCoefficients();
  }
  void setFrequency(float p_freq) {
    jassert(m_samplerate > 0);
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
  //float m_samplerate = -1;
  //float m_one_over_samplerate;
};
