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

#include "SEMFilter12.h"

SEMFilter12::SEMFilter12() {
	m_alpha_0 = 1.0;
	m_alpha   = 1.0;
	m_rho     = 1.0;

	reset();

	m_transition       = -1.;
	m_resonance        = 0.5f;
	m_resonance_modded = m_resonance;
}

SEMFilter12::~SEMFilter12() {
}

void SEMFilter12::reset() {
	m_z_1 = 0.0;
	m_z_2 = 0.0;
}

void SEMFilter12::update() {
	Filter::update();

	//recalc coeffs only if
	// freq changed
	// res modded
	// res changed (m_last_freq_modded = -1)
	// res samplerate (m_last_freq_modded = -1)
	if (m_freq_modded == m_last_freq_modded && !(*m_res_mod)) {
		return;
	}
	m_last_freq_modded = m_freq_modded;

	double wd = 2 * M_PI * m_freq_modded;
	//double T = 1.0 / m_samplerate;
	double wa = (2 * m_samplerate) * juce::dsp::FastMathApproximations::tan(wd * m_one_over_samplerate * 0.5);
	double g  = wa * m_one_over_samplerate * 0.5;

	m_resonance_modded = m_resonance + (*m_res_mod) * 24.5;
	m_resonance_modded = m_resonance_modded > 25 ? 25 : m_resonance_modded;
	m_resonance_modded = m_resonance_modded < 0.5 ? 0.5 : m_resonance_modded;

	double r = 1.0 / (2.0 * m_resonance_modded);

	m_alpha_0 = 1.0 / (1.0 + 2.0 * r * g + g * g);
	m_alpha   = g;
	m_rho     = 2.0 * r + g;
}

double SEMFilter12::doFilter(double xn) {
    jassert(m_samplerate > 0);

	double hpf = m_alpha_0 * (xn - m_rho * m_z_1 - m_z_2);
	double bpf = m_alpha * hpf + m_z_1;

	double lpf = m_alpha * bpf + m_z_2;
	double r   = 1.0 / (2.0 * m_resonance_modded);
	double bsf = xn - 2.0 * r * bpf;

	m_z_1 = m_alpha * hpf + bpf;
	m_z_2 = m_alpha * bpf + lpf;

	float transition_modded = m_transition + *m_transition_mod * 2;
	transition_modded       = transition_modded > 1 ? 1 : transition_modded;
	transition_modded       = transition_modded < -1 ? -1 : transition_modded;

	if (transition_modded < 0) {
		xn = (1 + transition_modded) * bsf - transition_modded * lpf;
	} else {
		xn = transition_modded * hpf + (1 - transition_modded) * bsf;
	}

	return xn;
}

void SEMFilter12::setResControl(double p_res) {
	m_resonance        = 24.5 * p_res * p_res * p_res * p_res + 0.5;
	m_last_freq_modded = -1; // to signal recalculation of filter coeffs in update()
}

void SEMFilter12::setSampleRate(double p_sr) {
	Filter::setSampleRate(p_sr);
	m_last_freq_modded = -1; // to signal recalculation of filter coeffs in update()
}
