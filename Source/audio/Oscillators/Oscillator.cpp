#include "Oscillator.h"

// --- constuction
Oscillator::Oscillator(void)
{
	// --- initialize variables
	m_samplerate = 44100;
	//m_note_on = true;
	m_modulo = 0.0;
	m_increment = 0.0;
	m_osc_freq_base = OSC_FO_DEFAULT; // GUI
	m_amplitude = 1.0;		   // default ON
	m_osc_freq_modded = OSC_FO_DEFAULT;

	// --- continue inits
	m_mod_amp = 1.0; // note default to 1 to avoid silent osc
	m_mod_freq_lin = 0.0;
	m_mod_freq_exp = 0.0;
	m_mod_pitch_bend = 0.0;
	m_octave = 0.0;

	m_semitones = 0.0;
	m_cent = 0.0;

	// --- for hard sync
	m_buddy_oscillator = nullptr;
	m_is_master_osc = false;
}

// --- destruction
Oscillator::~Oscillator(void)
{
}

// --- VIRTUAL FUNCTION; base class implementations
void Oscillator::reset()
{
	// --- Pitched modulos, wavetables start at 0.0
	m_modulo = 0.0;

	// modulation variables
	m_mod_amp = 1.0; // note default to 1 to avoid silent osc
	m_mod_pitch_bend = 0.0;
	m_mod_freq_exp = 0.0;
	m_mod_freq_lin = 0.0;
}
