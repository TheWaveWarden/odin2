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

#include "../JuceLibraryCode/JuceHeader.h"


class BiquadFilter {
public:
  BiquadFilter();
  ~BiquadFilter();

  inline void reset() { m_z1 = m_z2 = 0.; }

  void setFilterCoefficients(double p_a1, double p_a2, double p_b0, double p_b1,
                             double p_b2);

  virtual double doFilter(double p_input);

  inline void setSampleRate(float p_samplerate){ 
		m_samplerate = p_samplerate;
		m_one_over_samplerate = 1. / p_samplerate;
	}

protected:
  // direkt form coefficients. note that in the normalized form, a0 is 1
  double m_a1;
  double m_a2;
  double m_b0;
  double m_b1;
  double m_b2;

  // buffer elements
  double m_z1 = 0.;
  double m_z2 = 0.;

  float m_samplerate = -1;
  float m_one_over_samplerate;
};
