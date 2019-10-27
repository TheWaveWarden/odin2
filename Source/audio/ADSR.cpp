#include "ADSR.h"

#include "../JuceLibraryCode/JuceHeader.h"

ADSREnvelope::ADSREnvelope() {
	// m_cheap_functions = CheapFunctions::getInstance();
}

ADSREnvelope::~ADSREnvelope() {
}

float ADSREnvelope::doEnvelope() {
	switch (m_current_section) {
	case ADSR_SECTION_FINISHED:
		m_last_actual_value = 0.f;
		return 0.f;
		break;

	case ADSR_SECTION_INIT:
		m_attack_start_value = 0.f;
		m_current_value      = 0.;
		m_current_section    = ADSR_SECTION_ATTACK;
		// ! no break is intentional
	case ADSR_SECTION_ATTACK:
		m_attack_modded = m_attack;
		if (*m_attack_mod) {
			m_attack_modded *= calcModFactor(*m_attack_mod);
		}
		m_current_value += 1. / m_samplerate / m_attack_modded;
		if (m_current_value >= 1) {
			m_current_value   = 1;
			m_current_section = ADSR_SECTION_DECAY;
		}
		m_last_actual_value = m_current_value;

		return m_last_actual_value;
		break;

	case ADSR_SECTION_DECAY:
		m_decay_modded = m_decay;
		if (*m_decay_mod) {
			m_decay_modded *= calcModFactor(*m_decay_mod);
		}

		// just decay to zero but return scaled
		m_decay_factor = calcDecayFactor(m_decay_modded);
		m_current_value *= m_decay_factor;
		m_sustain_modded = m_sustain + *m_sustain_mod;
		m_sustain_modded        = m_sustain_modded < 0 ? 0 : m_sustain_modded;
		m_sustain_modded        = m_sustain_modded > 1 ? 1 : m_sustain_modded;
		if (m_current_value < MIN_DECAY_RELEASE_VAL) {
			if (m_loop) {
				m_current_section    = ADSR_SECTION_ATTACK;
				m_attack_start_value = m_sustain_modded;
				m_current_value      = m_sustain_modded + (1 - m_sustain_modded) * m_current_value;

				m_last_actual_value = m_sustain_modded;
				return m_last_actual_value;
			} else {
				m_current_section = ADSR_SECTION_SUSTAIN;
				m_current_value   = 0; // why?
			}
		}
		m_last_actual_value = m_sustain_modded + (1 - m_sustain_modded) * m_current_value;
		return m_last_actual_value;
		break;

	case ADSR_SECTION_SUSTAIN:
		m_sustain_modded = m_sustain + *m_sustain_mod;
		m_sustain_modded        = m_sustain_modded < 0 ? 0 : m_sustain_modded;
		m_sustain_modded        = m_sustain_modded > 1 ? 1 : m_sustain_modded;
		// just return sustain here
		if (m_loop) {
			m_current_section    = ADSR_SECTION_ATTACK;
			m_attack_start_value = m_sustain_modded;
			m_current_value      = m_sustain_modded + (1 - m_sustain_modded) * m_current_value;
		}
		m_last_actual_value = m_sustain_modded;
		return m_last_actual_value;
		break;

	case ADSR_SECTION_RELEASE:
		m_release_modded = m_release;
		if (*m_release_mod) {
			m_release_modded *= calcModFactor(*m_release_mod * 2);
		}

		m_release_factor = calcReleaseFactor(m_release_modded);
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
		break;
	default: //should never get here
		return 0.f;
		break;
	}
}

void ADSREnvelope::startRelease() {
	if (m_current_section == ADSR_SECTION_RELEASE) {
		return;
	}
	m_sustain_modded = m_sustain + *m_sustain_mod;
	m_sustain_modded        = m_sustain_modded < 0 ? 0 : m_sustain_modded;
	m_sustain_modded        = m_sustain_modded > 1 ? 1 : m_sustain_modded;
	if (m_current_section == ADSR_SECTION_DECAY) {
		m_release_start_value = m_current_value * (1. - m_sustain_modded) + m_sustain_modded;
	} else if (m_current_section == ADSR_SECTION_SUSTAIN) {
		m_release_start_value = m_sustain_modded;
	} else {
		m_release_start_value = m_current_value;
	}

	m_current_value   = 1.;
	m_current_section = ADSR_SECTION_RELEASE;
}

int ADSREnvelope::getCurrentSection() {
	return m_current_section;
}
