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

#include "Amplifier.h"

// void Amplifier::incWriteIndex() {
// 	if (++m_write_index >= WIDTH_DELAY_SAMPLES) {
// 		m_write_index = 0;
// 	}
// }

void Amplifier::doAmplifier(float p_in, float &po_left_out, float &po_right_out) {
	jassert(m_samplerate > 0);

	m_gain_smooth = m_gain_smooth * GAIN_SMOOTHIN_FACTOR + (1.f - GAIN_SMOOTHIN_FACTOR) * (m_gain);
	m_pan_smooth  = m_pan_smooth * PAN_SMOOTHIN_FACTOR + (1.f - PAN_SMOOTHIN_FACTOR) * m_pan;

	//gain && modulation
	float gain_modded = m_gain_smooth;
	if (*m_gain_mod) {
		if (*m_gain_mod < 0.f) {
			//negative modulation just modulates down to -inf dB
			gain_modded = m_gain_smooth * (1.f + *m_gain_mod);
			gain_modded = gain_modded < 0 ? 0 : gain_modded;
		} else {
			if (m_gain_smooth > MINUS_12_dB_GAIN) {
				// volume level above -12dB, modulate to plus 12 dB
				gain_modded *= pow(PLUS_12_dB_GAIN, *m_gain_mod);
				gain_modded = gain_modded > PLUS_12_dB_GAIN ? PLUS_12_dB_GAIN : gain_modded;
			} else {
				// if volume level is below -12dB then just modulate up to 0dB
				gain_modded += (1.f - gain_modded) * *m_gain_mod;
				gain_modded = gain_modded > PLUS_12_dB_GAIN ? PLUS_12_dB_GAIN : gain_modded;
			}
		}
	}

	p_in *= gain_modded * *m_unison_gain_reduction_pointer;

	// apply velocity
	float vel_modded = m_vel_amount + *(m_vel_mod);
	vel_modded       = vel_modded < 0 ? 0 : vel_modded;
	vel_modded       = vel_modded > 1 ? 1 : vel_modded;
	p_in *= 1.f - vel_modded * 1.f + vel_modded * m_MIDI_vel;

	// do panning
	float pan_modded = m_pan_smooth + *m_pan_mod + *m_unison_pan_position_pointer * m_unison_pan_amount;
	pan_modded       = pan_modded < -1 ? -1 : pan_modded;
	pan_modded       = pan_modded > 1 ? 1 : pan_modded;
	if (pan_modded >= 0.f) {
		po_right_out = p_in;
		po_left_out  = (1.f - pan_modded) * p_in;
	} else {
		po_right_out = (pan_modded + 1.f) * p_in;
		po_left_out  = p_in;
	}
}