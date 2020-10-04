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

#include "FormantFilter.h"

#define FORMANT_OUTPUT_SCALAR 0.35f

FormantFilter::FormantFilter() {
	reset();
	updateParabolas();
	setFreqModPointer(&m_zero_dummy);
}

FormantFilter::~FormantFilter() {
}

void FormantFilter::reset() {
	m_resonator1.reset();
	m_resonator2.reset();
}

void FormantFilter::update() {

	float vel_modded = m_vel_mod_amount + *m_vel_mod_mod < 0 ? 0 : m_vel_mod_amount + *m_vel_mod_mod;

	float transition_modded = m_transition + *m_transition_mod + vel_modded * (float)m_MIDI_velocity / 127.f +
	                          (m_env_mod_amount + *m_env_mod_mod) * m_env_value;
							  
	CLAMP(0, transition_modded, 1);

	// set freq from parabolas
	m_resonator1.setFrequency(m_a0 * transition_modded * transition_modded + m_b0 * transition_modded + m_c0);
	m_resonator2.setFrequency(m_a1 * transition_modded * transition_modded + m_b1 * transition_modded + m_c1);
}

void FormantFilter::updateParabolas() {
	float f0 = m_formant_list[m_vowel_left][0];
	float f1 = m_formant_list[m_vowel_right][0];
	float f2 = f0 * pow(f1 / f0, 0.5);

	m_a0 = 2 * f1 - 4 * f2 + 2 * f0;
	m_b0 = 4 * f2 - 3 * f0 - f1;
	m_c0 = f0;

	f0 = m_formant_list[m_vowel_left][1];
	f1 = m_formant_list[m_vowel_right][1];
	f2 = f0 * pow(f1 / f0, 0.5);

	m_a1 = 2 * f1 - 4 * f2 + 2 * f0;
	m_b1 = 4 * f2 - 3 * f0 - f1;
	m_c1 = f0;
}

void FormantFilter::setTransition(float p_trans) {
	m_transition = p_trans;
}

double FormantFilter::doFilter(double p_input) {
    jassert(m_samplerate > 0);

	double out           = m_resonator1.doFilter(m_resonator2.doFilter(p_input));

	return out * m_samplerate_gain_compensation * FORMANT_OUTPUT_SCALAR;
}