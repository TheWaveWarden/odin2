#include "Filter.h"

// --- construction
Filter::Filter(void) {
	// --- defaults
	m_samplerate  = 44100;
	m_res_base    = 1.0; // Q is 1 to 10 on GUI
	m_freq_modded = FILTER_FC_DEFAULT;
	m_res_modded  = FILTER_Q_DEFAULT;
	m_freq_base   = FILTER_FC_DEFAULT;
	// --- clear
	m_mod_frequency = 0.0;
}

Filter::~Filter(void) {
}

// --- flush buffers
void Filter::reset() {
}
