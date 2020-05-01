
#include "RingModulator.h"

#define RINGMOD_ENV_MOD_SEMITONES_MAX 48

RingModulator::RingModulator() {
	m_glide_mod     = &m_modulation_dummy;
	m_pitch_mod_exp = &m_modulation_dummy;
	m_pitch_mod_lin = &m_modulation_dummy;
	//m_vol_mod       = &m_modulation_dummy;
	m_pitchbend     = &m_modulation_dummy;

	m_nr_of_wavetables = 1;
}

float RingModulator::doRingModulator(float p_input) {
    jassert(m_samplerate > 0);

	//float vol_mod_factor = (*m_vol_mod) > 0 ? 1.f + 4 * (*m_vol_mod) : (1.f + *m_vol_mod);
	//vol_mod_factor       = vol_mod_factor > VOL_MOD_UPPER_LIMIT ? VOL_MOD_UPPER_LIMIT : vol_mod_factor;

	float amount_modded = m_amount + *m_ringmod_amount_mod;
	amount_modded       = amount_modded > 1 ? 1 : amount_modded;
	amount_modded       = amount_modded < 0 ? 0 : amount_modded;

	return p_input * (doOscillate() * amount_modded + (1.f - amount_modded));// * vol_mod_factor;
}

void RingModulator::reset() {
}

int RingModulator::getTableIndex() {
	// sine is always the same
	return 0;
}

void RingModulator::update() {
	// copy and paste from wt1d and oscillator and do manually

	float glide_input_modded = m_glide + *(m_glide_mod);

	glide_input_modded  = glide_input_modded > 1 ? 1 : glide_input_modded;
	double glide_modded = calculateGlide(glide_input_modded);

	m_osc_freq_base = m_osc_freq_glide_target * (1.f - glide_modded) + (glide_modded)*m_osc_freq_base;

	float kbd_modded = m_kbd_mod_amount + *m_kbd_mod_mod < 0 ? 0 : m_kbd_mod_amount + *m_kbd_mod_mod;
	float vel_modded = m_vel_mod_amount + *m_vel_mod_mod < 0 ? 0 : m_vel_mod_amount + *m_vel_mod_mod;

	// --- do the  complete frequency mod
	m_osc_freq_modded =
	    m_osc_freq_base * pitchShiftMultiplier((*m_pitch_mod_exp) * OSC_EXP_MOD_RANGE + kbd_modded * m_MIDI_note +
	                                           (m_env_value * (m_env_mod_amount + *m_env_mod_mod) +
	                                            vel_modded * (float)m_MIDI_velocity / 127.f) *
	                                               RINGMOD_ENV_MOD_SEMITONES_MAX);
	// --- apply linear FM (not used in book projects)
	m_osc_freq_modded += (*m_pitch_mod_lin) * m_osc_freq_modded * 2 + m_mod_freq_lin;

	// --- bound Fo (can go outside for FM/PM mod)
	//     +/- 20480 for FM/PM
	if (m_osc_freq_modded > OSC_FO_MAX)
		m_osc_freq_modded = OSC_FO_MAX;
	if (m_osc_freq_modded < -OSC_FO_MAX)
		m_osc_freq_modded = -OSC_FO_MAX;

	// --- calculate increment (a.k.a. phase a.m_k.a. phaseIncrement, etc...)
	m_increment       = m_osc_freq_modded * m_one_over_samplerate;
	m_wavetable_inc   = WAVETABLE_LENGTH * m_increment;
	m_sub_table_index = getTableIndex();

	// set wavetable pointer
	m_current_table = m_wavetable_pointers[m_wavetable_index][m_sub_table_index];
}