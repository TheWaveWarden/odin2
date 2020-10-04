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

#include "ADSR.h"

#include "../JuceLibraryCode/JuceHeader.h"

ADSREnvelope::ADSREnvelope() {
	// m_cheap_functions = CheapFunctions::getInstance();
}

ADSREnvelope::~ADSREnvelope() {
}

float ADSREnvelope::doEnvelope() {
	jassert(m_samplerate > 0);

	switch (m_current_section) {
	case ADSR_SECTION_FINISHED:
		m_last_actual_value = 0.f;
		return 0.f;
		break;

	case ADSR_SECTION_INIT:
		m_attack_start_value = 0.f;
		m_current_value      = 0.;
		m_current_section    = ADSR_SECTION_ATTACK;
		// ! intentional fallthrough
	case ADSR_SECTION_ATTACK: {
		double attack_modded = m_attack;
		if (*m_attack_mod) {
			//doing exp and additional - only additional is bad for high values
			attack_modded = attack_modded * calcModFactor(*m_attack_mod) + *m_attack_mod * 0.3f;
			attack_modded = attack_modded < ATTACK_LOWER_LIMIT ? ATTACK_LOWER_LIMIT : attack_modded;
		}
		m_current_value += 1. / (m_samplerate * attack_modded);
		if (m_current_value >= 1) {
			m_current_value   = 1;
			m_current_section = ADSR_SECTION_DECAY;
		}
		m_last_actual_value = m_current_value;

		return m_last_actual_value;
	} break;

	case ADSR_SECTION_DECAY: {
		double decay_modded = m_decay;
		if (*m_decay_mod) {
			decay_modded = decay_modded * calcModFactor(*m_decay_mod) + *m_decay_mod * 0.3f;
			decay_modded = decay_modded < DECAY_LOWER_LIMIT ? DECAY_LOWER_LIMIT : decay_modded;
		}

		// just decay to zero but return scaled
		m_decay_factor = calcDecayFactor(decay_modded);
		m_current_value *= m_decay_factor;
		double sustain_modded = m_sustain + *m_sustain_mod;
		sustain_modded        = sustain_modded < 0 ? 0 : sustain_modded;
		sustain_modded        = sustain_modded > 1 ? 1 : sustain_modded;
		if (m_current_value < MIN_DECAY_RELEASE_VAL) {
			if (m_loop) {
				m_current_section    = ADSR_SECTION_ATTACK;
				m_attack_start_value = sustain_modded;
				m_current_value      = sustain_modded + (1 - sustain_modded) * m_current_value;

				m_last_actual_value = sustain_modded;
				return m_last_actual_value;
			} else {
				m_current_section = ADSR_SECTION_SUSTAIN;
				m_current_value   = 0; // why?
			}
		}
		m_last_actual_value = sustain_modded + (1 - sustain_modded) * m_current_value;
		return m_last_actual_value;
	} break;

	case ADSR_SECTION_SUSTAIN: {
		double sustain_modded = m_sustain + *m_sustain_mod;
		sustain_modded        = sustain_modded < 0 ? 0 : sustain_modded;
		sustain_modded        = sustain_modded > 1 ? 1 : sustain_modded;
		// just return sustain here
		if (m_loop) {
			m_current_section    = ADSR_SECTION_ATTACK;
			m_attack_start_value = sustain_modded;
			m_current_value      = sustain_modded + (1 - sustain_modded) * m_current_value;
		}
		m_last_actual_value = sustain_modded;
		return m_last_actual_value;
	} break;

	case ADSR_SECTION_RELEASE: {
		double release_modded = m_release;
		if (*m_release_mod) {
			release_modded = release_modded * calcModFactor(*m_release_mod * 2) + *m_release_mod * 0.3f;
			release_modded = release_modded < RELEASE_LOWER_LIMIT ? RELEASE_LOWER_LIMIT : release_modded;
		}

		m_release_factor = calcReleaseFactor(release_modded);
		// again just decay from 1 to 0 and output scaled version
		m_current_value *= m_release_factor;
		if (m_current_value < MIN_DECAY_RELEASE_VAL) {
			// envelope ends here...
			m_current_value = 0.;
			onEnvelopeEnd();
			m_current_section = ADSR_SECTION_FINISHED;
		}
		m_last_actual_value = m_current_value * m_release_start_value;
		return m_last_actual_value;
	} break;
	default: //should never get here
		return 0.f;
		break;
	}
}

void ADSREnvelope::startRelease() {
	if (m_current_section == ADSR_SECTION_RELEASE) {
		return;
	}
	double sustain_modded = m_sustain + *m_sustain_mod;
	sustain_modded        = sustain_modded < 0 ? 0 : sustain_modded;
	sustain_modded        = sustain_modded > 1 ? 1 : sustain_modded;
	if (m_current_section == ADSR_SECTION_DECAY) {
		m_release_start_value = m_current_value * (1. - sustain_modded) + sustain_modded;
	} else if (m_current_section == ADSR_SECTION_SUSTAIN) {
		m_release_start_value = sustain_modded;
	} else {
		m_release_start_value = m_current_value;
	}

	m_current_value   = 1.;
	m_current_section = ADSR_SECTION_RELEASE;
}

int ADSREnvelope::getCurrentSection() {
	return m_current_section;
}
