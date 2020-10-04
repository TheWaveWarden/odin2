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

#include "VAOnePoleFilter.h"

VAOnePoleFilter::VAOnePoleFilter() {
	// init
	m_alpha   = 1.0;
	m_beta    = 0.0;
	m_gamma   = 1.0;
	m_delta   = 0.0;
	m_epsilon = 0.0;

	m_z_1 = 0.0;
	m_a_0 = 1.0;

	m_feedback = 0.0;

	// always set the default!
	m_is_lowpass = true;

	// flush
	reset();
}

VAOnePoleFilter::~VAOnePoleFilter() {
}

// recalc coeffs
void VAOnePoleFilter::update() {
	// base class does modulation, changes m_fFc
	Filter::update();

	double wd = 2 * M_PI * m_freq_modded;
	double T  = 1 / m_samplerate;
	double wa = (2 / T) * tan(wd * T / 2);
	double g  = wa * T / 2;

	m_alpha = g / (1.0 + g);
}

// do the filter
double VAOnePoleFilter::doFilter(double xn) {
    jassert(m_samplerate > 0);

	// for diode filter support
	xn = xn * m_gamma + m_feedback + m_epsilon * getFeedbackOutput();
	// calculate v(n)
	double vn = (m_a_0 * xn - m_z_1) * m_alpha;
	// form LP output
	double lpf = vn + m_z_1;
	// update memory
	m_z_1 = vn + lpf;
	if (m_is_lowpass) {
		return lpf;
	}
	return xn - lpf;
}
