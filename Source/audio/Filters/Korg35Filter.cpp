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

// The code for this filter is based on the diagrams and implementations in Will Pirkles book
// "Designing Software Synthesizer Plug-Ins in C++". Find out more on https://willpirkle.com

#include "Korg35Filter.h"

Korg35Filter::Korg35Filter() {
	m_is_lowpass = true;

	// init
	m_k     = 0.01;
	m_alpha = 0.0;

	m_LPF1.setLP();
	m_LPF2.setLP();
	m_HPF1.setHP();
	m_HPF2.setHP();

	reset();
}

Korg35Filter::~Korg35Filter() {
}

void Korg35Filter::reset() {
	m_LPF1.reset();
	m_LPF2.reset();
	m_HPF1.reset();
	m_HPF2.reset();
}

void Korg35Filter::update() {
	Filter::update();

	//recalc filter coeffs only if
	// freq_modded changed
	// resonance modulation
	// sample rate hast changed (m_last_freq_modded = -1)
	// resonance changed (... = -1)
	// filter type has changed (... = -1)

	if (m_freq_modded == m_last_freq_modded && !(*m_res_mod)) {
		return;
	}
	m_last_freq_modded = m_freq_modded;

	// BZT
	double wd = 2 * 3.141592653 * m_freq_modded;
	//double t = 1.0 / m_samplerate;
	double wa = (2.0 * m_samplerate) * juce::dsp::FastMathApproximations::tan(wd * m_one_over_samplerate * 0.5);
	double g  = wa * m_one_over_samplerate * 0.5;

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
    jassert(m_samplerate > 0);

	double y;
	if (m_is_lowpass) {
		double y1  = m_LPF1.doFilter(xn);
		double s35 = m_LPF2.getFeedbackOutput() + m_HPF1.getFeedbackOutput();
		double u   = m_alpha * (y1 + s35);

		y = m_k_modded * m_LPF2.doFilter(u);
		m_HPF1.doFilter(y);
	} else {
		double y1  = m_HPF1.doFilter(xn);
		double s35 = m_HPF2.getFeedbackOutput() + m_LPF1.getFeedbackOutput();
		double u   = m_alpha * (y1 + s35);

		y = m_k_modded * u;
		m_LPF1.doFilter(m_HPF2.doFilter(y));
	}
	y /= m_k_modded;

	// make this one a bit easier (3.f), its very aggresive
	applyOverdrive(y, 3.f);

	return y;
}

void Korg35Filter::setResControl(double res) {
	// note: m_k must never be zero else division by zero
	// note2 original was 1.99 but dont want self oscillation
	m_k                = res * 1.95 + 0.01;
	m_last_freq_modded = -1; // to signal recalculation of filter coeffs in update()
}

void Korg35Filter::setSampleRate(double p_sr) {
	Filter::setSampleRate(p_sr);
	m_LPF1.setSampleRate(p_sr);
	m_LPF2.setSampleRate(p_sr);
	m_HPF1.setSampleRate(p_sr);
	m_HPF2.setSampleRate(p_sr);
	m_last_freq_modded = -1; // to signal recalculation of filter coeffs in update()
}
