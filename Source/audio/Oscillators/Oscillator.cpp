#include "Oscillator.h"

// --- constuction
Oscillator::Oscillator() {
}

// --- destruction
Oscillator::~Oscillator() {
}

// --- VIRTUAL FUNCTION; base class implementations
void Oscillator::reset() {
	// modulation variables
	m_mod_freq_exp = 0.0;
	m_mod_freq_lin = 0.0;
}
