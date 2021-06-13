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

// The code for this filter is based on the diagrams and implementations in Will Pirkles book
// "Designing Software Synthesizer Plug-Ins in C++". Find out more on https://willpirkle.com

#include "DiodeFilter.h"

DiodeFilter::DiodeFilter() {
	// init
	m_k     = 0.0;
	m_gamma = 0.0;

	// feedback scalars
	m_sg1 = 0.0;
	m_sg2 = 0.0;
	m_sg3 = 0.0;
	m_sg4 = 0.0;

	m_LPF1.setLP(); //LP
	m_LPF2.setLP(); //LP
	m_LPF3.setLP(); //LP
	m_LPF4.setLP(); //LP

	reset();
}

DiodeFilter::~DiodeFilter() {
}

void DiodeFilter::reset() {
	m_LPF1.reset();
	m_LPF2.reset();
	m_LPF3.reset();
	m_LPF4.reset();
}

void DiodeFilter::update() {
	//modulation
	Filter::update();

	//recalculate filter coefficients only if:
	// filter freq changed
	// sample rate changed (m_last_freq_modded set to -1)

	if (m_last_freq_modded == m_freq_modded) {
		return;
	}
	m_last_freq_modded = m_freq_modded;

	//calc alphas
	double wd = 2 * 3.141592653 * m_freq_modded;
	//double t = 1.0 / m_samplerate;
	double wa = (2.0 * m_samplerate) * juce::dsp::FastMathApproximations::tan(wd * m_one_over_samplerate * 0.5);
	double g  = wa * m_one_over_samplerate / 2.0;

	double G4 = 0.5 * g / (1.0 + g);
	double G3 = 0.5 * g / (1.0 + g - 0.5 * g * G4);
	double G2 = 0.5 * g / (1.0 + g - 0.5 * g * G3);
	double G1 = g / (1.0 + g - g * G2);
	m_gamma   = G4 * G3 * G2 * G1;

	m_sg1 = G4 * G3 * G2;
	m_sg2 = G4 * G3;
	m_sg3 = G4;
	m_sg4 = 1.0;

	double G = g / (1.0 + g);

	m_LPF1.m_alpha = G;
	m_LPF2.m_alpha = G;
	m_LPF3.m_alpha = G;
	m_LPF4.m_alpha = G;

	m_LPF1.m_beta = 1.0 / (1.0 + g - g * G2);
	m_LPF2.m_beta = 1.0 / (1.0 + g - 0.5 * g * G3);
	m_LPF3.m_beta = 1.0 / (1.0 + g - 0.5 * g * G4);
	m_LPF4.m_beta = 1.0 / (1.0 + g);

	m_LPF1.m_delta = g;
	m_LPF2.m_delta = 0.5 * g;
	m_LPF3.m_delta = 0.5 * g;
	m_LPF4.m_delta = 0.0;

	m_LPF1.m_gamma = 1.0 + G1 * G2;
	m_LPF2.m_gamma = 1.0 + G2 * G3;
	m_LPF3.m_gamma = 1.0 + G3 * G4;
	m_LPF4.m_gamma = 1.0;

	m_LPF1.m_epsilon = G2;
	m_LPF2.m_epsilon = G3;
	m_LPF3.m_epsilon = G4;
	m_LPF4.m_epsilon = 0.0;

	m_LPF1.m_a_0 = 1.0;
	m_LPF2.m_a_0 = 0.5;
	m_LPF3.m_a_0 = 0.5;
	m_LPF4.m_a_0 = 0.5;
}

double DiodeFilter::doFilter(double xn) {
    jassert(m_samplerate > 0);

	m_LPF4.m_feedback = 0.0;
	m_LPF3.m_feedback = m_LPF4.getFeedbackOutput();
	m_LPF2.m_feedback = m_LPF3.getFeedbackOutput();
	m_LPF1.m_feedback = m_LPF2.getFeedbackOutput();

	double sigma = m_sg1 * m_LPF1.getFeedbackOutput() + m_sg2 * m_LPF2.getFeedbackOutput() +
	               m_sg3 * m_LPF3.getFeedbackOutput() + m_sg4 * m_LPF4.getFeedbackOutput();

	float k_modded = m_k + *m_res_mod * 16;
	k_modded       = k_modded > 16 ? 16 : k_modded;
	k_modded       = k_modded < 0 ? 0 : k_modded;

	// for passband gain compensation:
	xn *= 1.0f + 0.3f * k_modded;

	double u = (xn - k_modded * sigma) / (1.0 + k_modded * m_gamma);

	double output = m_LPF4.doFilter(m_LPF3.doFilter(m_LPF2.doFilter(m_LPF1.doFilter(u))));

	applyOverdrive(output);

	return output;
}

void DiodeFilter::setResControl(double res) {
	m_k = 16.0 * res;
}

void DiodeFilter::setSampleRate(double p_sr) {
	Filter::setSampleRate(p_sr);
	m_LPF1.setSampleRate(p_sr);
	m_LPF2.setSampleRate(p_sr);
	m_LPF3.setSampleRate(p_sr);
	m_LPF4.setSampleRate(p_sr);
	m_last_freq_modded = -1; //to signal recalcualtion of filter coeffs in update()
}
