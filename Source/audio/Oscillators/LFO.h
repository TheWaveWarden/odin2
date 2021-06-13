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

#pragma once

#include "WavetableOsc1D.h"

class LFO : public WavetableOsc1D {

public:
	LFO() {
		//always use lowest table, since this is LFO and we really dont care about aliasing
		m_sub_table_index = 0;

		//get first random SH value
		m_SH_value = getBipolarRandom();

		m_osc_freq_base = 1.f;
	}
	~LFO() {
	}

	void loadWavetables() override;
	float doOscillate() override;

	inline void update() override {

		// --- do the  complete frequency mod
		if (*m_pitch_mod_exp) {
			m_osc_freq_modded = m_osc_freq_base * pitchShiftMultiplier((*m_pitch_mod_exp) * 48);
		} else {
			m_osc_freq_modded = m_osc_freq_base;
		}

		m_osc_freq_modded = m_osc_freq_modded > OSC_FO_MAX ? OSC_FO_MAX : m_osc_freq_modded;

		m_increment     = m_osc_freq_modded / m_samplerate;
		m_wavetable_inc = WAVETABLE_LENGTH * m_increment;

		// set wavetable pointer
		m_current_table = m_wavetable_pointers[m_wavetable_index][m_sub_table_index];
	}

	void setSHActive(bool p_active) {
		m_SH_active = p_active;
	}

	float getBipolarRandom() {
		float white_noise = (float)rand();
		return 2 * (white_noise / RAND_MAX) - 1;
	}

	float doSampleHold();

	void setFreqBPM(float p_BPM) {
		setBaseFrequency(p_BPM / m_synctime_ratio / 240.f);
	}
	void setSynctimeNumerator(float p_value) {
		m_synctime_numerator = p_value;
		m_synctime_ratio     = p_value / m_synctime_denominator;
	}
	void setSynctimeDenominator(float p_value) {
		m_synctime_denominator = p_value;
		m_synctime_ratio       = m_synctime_numerator / p_value;
	}

protected:
	float m_synctime_numerator   = 3.f;
	float m_synctime_denominator = 16.f;
	float m_synctime_ratio       = 3.f / 16.f;

	float m_SH_active = false;
	float m_SH_value;

	inline int getTableIndex() override {
		return 0;
	}
};