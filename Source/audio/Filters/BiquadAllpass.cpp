/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 - 2021 TheWaveWarden
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

#include "BiquadAllpass.h"
#include <cmath>

BiquadAllpass::BiquadAllpass() {
	//stays the same for all configurations
	m_b2 = 1.;
}

BiquadAllpass::~BiquadAllpass() {
}

void BiquadAllpass::setFrequency(float p_frequency) {
	//Transfer function:
	//H(z) = (1/r² - 2/r*cos * z^-1 + z^-2)/(1 - 2/r*cos * z^-1 + 1/r² * z^-2)

	//convert freq to radial freq
	float freq_rad = p_frequency * m_one_over_samplerate * 2 * PI;

	m_b1 = -2.f / m_radius * juce::dsp::FastMathApproximations::cos(freq_rad);
	m_a1 = m_b1;
	m_b0 = 1.f / (m_radius * m_radius);
	m_a2 = m_b0;
}