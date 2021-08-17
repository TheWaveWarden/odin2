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

#include "FMOscillator.h"

FMOscillator::FMOscillator() {
	m_carrier_osc.setPitchModExpPointer(&m_zero_modulation_dummy);
	m_modulator_osc.setPitchModExpPointer(&m_zero_modulation_dummy);
	m_carrier_osc.setPitchModLinPointer(&m_zero_modulation_dummy);
	m_modulator_osc.setPitchModLinPointer(&m_zero_modulation_dummy);
	//m_carrier_osc.setVolModPointer(&m_zero_modulation_dummy);
	//m_modulator_osc.setVolModPointer(&m_zero_modulation_dummy);
	m_carrier_osc.setPitchBendPointer(&m_zero_modulation_dummy);
	m_modulator_osc.setPitchBendPointer(&m_zero_modulation_dummy);
	m_modulator_osc.setGlidePointer(&m_zero_modulation_dummy);
	m_carrier_osc.setGlidePointer(&m_zero_modulation_dummy);
}

FMOscillator::~FMOscillator() {
}

void FMOscillator::loadWavetables() {
	m_carrier_osc.loadWavetables();
	m_modulator_osc.loadWavetables();
}

void FMOscillator::update() {
	// do this Oscillator calculation first.
	Oscillator::update();

	// use modded freq of this osc for basefreq of child oscs
	//DBG(m_osc_freq_modded);
	m_carrier_osc.m_osc_freq_base           = m_osc_freq_modded;
	m_modulator_osc.m_osc_freq_base         = m_osc_freq_modded * m_modulator_ratio / m_carrier_ratio;
	m_carrier_osc.m_osc_freq_glide_target   = m_osc_freq_modded;
	m_modulator_osc.m_osc_freq_glide_target = m_modulator_osc.m_osc_freq_base;

	// DBG(m_osc_freq_modded);
	// DBG(m_osc_freq_base);
	// DBG("was mod base");

	if (*m_carrier_ratio_mod || *m_modulator_ratio_mod) {
		m_modulator_osc.m_osc_freq_base *= pitchShiftMultiplier((*m_modulator_ratio_mod - *m_carrier_ratio_mod) * 24);
		m_modulator_osc.m_osc_freq_glide_target = m_modulator_osc.m_osc_freq_base;
	}

	// now do modulator osc so we can use it as modultion
	m_modulator_osc.update();
	float mod_value = m_modulator_osc.doOscillate();

	float fm_amount_modded = m_FM_amount + *m_fm_mod;
	fm_amount_modded       = fm_amount_modded < 0 ? 0 : fm_amount_modded;

	// set mod and update
	if (m_FM_is_exponential) {
		m_carrier_osc.m_mod_freq_exp = mod_value * fm_amount_modded * EXP_FM_SEMITONES;
	} else {
		m_carrier_osc.m_mod_freq_lin = mod_value * 15 * m_osc_freq_modded * fm_amount_modded;
	}
	m_carrier_osc.update();
}

void FMOscillator::reset() {
	m_carrier_osc.reset();
	m_modulator_osc.reset();
}

float FMOscillator::doOscillate() {
    jassert(m_samplerate > 0);

	float out = m_carrier_osc.doOscillate();

	m_reset_flag = m_carrier_osc.m_reset_flag;
	if (m_reset_flag) {
		m_reset_position = m_carrier_osc.m_reset_position;
	}

	return out;
}