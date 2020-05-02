#include "PMOscillator.h"

PMOscillator::PMOscillator() {
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

PMOscillator::~PMOscillator() {
}

void PMOscillator::loadWavetables() {
	m_carrier_osc.loadWavetables();
	m_modulator_osc.loadWavetables();
}

void PMOscillator::update() {
	// do this Oscillator calculation first.
	Oscillator::update();

	// use modded freq of this osc for basefreq of child oscs
	m_carrier_osc.m_osc_freq_base           = m_osc_freq_modded;
	m_modulator_osc.m_osc_freq_base         = m_osc_freq_modded * m_modulator_ratio / m_carrier_ratio;
	m_carrier_osc.m_osc_freq_glide_target   = m_osc_freq_modded;
	m_modulator_osc.m_osc_freq_glide_target = m_modulator_osc.m_osc_freq_base;

	if (*m_carrier_ratio_mod || *m_modulator_ratio_mod) {
		m_modulator_osc.m_osc_freq_base *= pitchShiftMultiplier((*m_modulator_ratio_mod - *m_carrier_ratio_mod) * 24);
		m_modulator_osc.m_osc_freq_glide_target = m_modulator_osc.m_osc_freq_base;
	}

	// now do modulator osc so we can use it as modultion
	m_modulator_osc.update();
	float mod_value = m_modulator_osc.doOscillate();

	float pm_amount_modded = m_PM_amount + *m_pm_mod;
	// pm_amount_modded = pm_amount_modded < 0 ? 0 : pm_amount_modded;

	// set mod and update
	m_carrier_osc.setPhaseMod(pm_amount_modded * mod_value);
	m_carrier_osc.update();
}

void PMOscillator::reset() {
	m_carrier_osc.reset();
	m_modulator_osc.reset();
}

float PMOscillator::doOscillate() {
    jassert(m_samplerate > 0);

	float out = m_carrier_osc.doOscillate();

	m_reset_flag = m_carrier_osc.m_reset_flag;
	if (m_reset_flag) {
		m_reset_position = m_carrier_osc.m_reset_position;
	}

	return out;
}
