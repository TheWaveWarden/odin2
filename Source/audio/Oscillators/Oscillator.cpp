#include "Oscillator.h"

Oscillator::Oscillator() {
	m_unison_detune_factor_pointer = &unison_detune_factor_dummy;
}

Oscillator::~Oscillator() {
}

void Oscillator::reset() {
	// modulation variables
	m_mod_freq_exp = 0.0;
	m_mod_freq_lin = 0.0;
}
