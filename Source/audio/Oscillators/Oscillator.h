#pragma once
//#include "../pluginconstants.h"
//#include "../synthfunctions.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include <cmath>

#define OSC_FO_MOD_RANGE 2          // 2 semitone default
#define OSC_HARD_SYNC_RATIO_RANGE 4 // 4
#define OSC_PITCHBEND_MOD_RANGE 12  // 12 semitone default
#define OSC_FO_MIN 20               // 20Hz
#define OSC_FO_MAX 20480            // 20.480kHz = 10 octaves up from 20Hz
#define OSC_FO_DEFAULT 440.0        // A5
#define OSC_EXP_MOD_RANGE 24

class Oscillator {

public:
	Oscillator(void);
	virtual ~Oscillator(void);

	// --- reset the modulo (required for master->slave operations)
	inline void resetModulo(double d = 0.0) {
		m_modulo = d;
	}
	
	virtual float doOscillate() = 0;
	virtual void setSampleRate(float p_samplerate) {

		// DBG("setsamplerate baseosc");
		m_samplerate          = p_samplerate;
		m_one_over_samplerate = 1. / p_samplerate;
	}
	virtual void reset();

	void voiceStart(bool p_randomize_phase) {
		if(p_randomize_phase){
			randomizePhase();
		}
		if (m_reset_active) {
			//DBG("RESET!");
			reset();
		}
	}

	static float pitchShiftMultiplier(float p_semitones) {
#ifndef BENCHMARK
		//note: fastexp was tested and showed a deviation of 0.75 cents in this range
		if (p_semitones < 48.f && p_semitones > -48.f) {
			return juce::dsp::FastMathApproximations::exp(0.05776226504 * p_semitones);
		}
#endif
		//0.05776226504 = ln(2)/12
		//apparently pow(a,b) is calculated as exp(ln(a)*b), hence this is faster
		return std::exp(0.05776226504 * p_semitones);
	}

	inline void setResetActive(bool p_reset) {
		m_reset_active = p_reset;
	}

	inline virtual void update() {
		//jassert(m_unison_detune_factor_pointer);

		float glide_input_modded = m_glide + *(m_glide_mod);

		glide_input_modded  = glide_input_modded > 1 ? 1 : glide_input_modded;
		double glide_modded = calculateGlide(glide_input_modded);

		m_osc_freq_base = m_osc_freq_glide_target * (1.f - glide_modded) + (glide_modded)*m_osc_freq_base;

		// just multiply the controls on
		m_osc_freq_modded = m_osc_freq_base * m_pitch_control_multiplier * *m_unison_detune_factor_pointer;
		// and only use pitchshift if needed!
		if ((*m_pitchbend) + (*m_pitch_mod_exp) + m_mod_freq_exp + m_mod_exp_other) {
			m_osc_freq_modded *= pitchShiftMultiplier((*m_pitchbend) + (*m_pitch_mod_exp) * OSC_EXP_MOD_RANGE +
			                                          m_mod_freq_exp + m_mod_exp_other);
		}

		// --- apply linear FM (not used in book projects)
		m_osc_freq_modded += (*m_pitch_mod_lin) * m_osc_freq_modded * 2 + m_mod_freq_lin;

		// --- bound Fo (can go outside for FM/PM mod)
		//     +/- 20480 for FM/PM
		if (m_osc_freq_modded > OSC_FO_MAX)
			m_osc_freq_modded = OSC_FO_MAX;
		if (m_osc_freq_modded < -OSC_FO_MAX)
			m_osc_freq_modded = -OSC_FO_MAX;

		// --- calculate increment (a.k.a. phase a.m_k.a. phaseIncrement, etc...)
		m_increment = m_osc_freq_modded * m_one_over_samplerate;
	}

	inline virtual void setGlideTargetFrequency(float p_freq) {
		m_osc_freq_glide_target = p_freq;
	}

	inline virtual void setBaseFrequency(float p_freq) {
		m_osc_freq_base = p_freq;
	}

	// moves the freq to glide target on osc change, otherwise after oscillator
	// change the oscs glide again
	void killGlide() {
		m_osc_freq_base = m_osc_freq_glide_target;
	}

	void setPitchModExpPointer(float *p_pointer) {
		m_pitch_mod_exp = p_pointer;
	}
	void setPitchModLinPointer(float *p_pointer) {
		m_pitch_mod_lin = p_pointer;
	}
	// void setVolModPointer(float *p_pointer) {
	// 	m_vol_mod = p_pointer;
	// }
	void setPitchBendPointer(float *p_pointer) {
		m_pitchbend = p_pointer;
	}
	void setGlidePointer(float *p_pointer) {
		m_glide_mod = p_pointer;
	}

	void setGlide(float p_glide) {
		m_glide = p_glide * 44800. / m_samplerate;
	}

	inline double calculateGlide(float p_glide) {
		if (p_glide < 0.01) {
			return 0.;
		} else {
			return 0.9985f + p_glide * 0.0014;
		}
	}

	virtual void randomizePhase() = 0;

public:

	bool m_reset_flag      = false; // this is used by other oscs to know when to sync
	float m_reset_position = 0.f;   //this is used by other oscs to know which pos to sync to

	bool m_reset_active = false;
	double m_osc_freq_glide_target; // the target freq after glide from MIDI note
	double m_osc_freq_base;         // oscillator frequency which glides towards glide target
	double m_modulo;                // modulo counter 0->1
	double m_increment;             // phase inc = fo/fs

	double m_mod_freq_exp = 0; // exp freq modulation input
	double m_mod_freq_lin = 0; // lin freq modulation input

	float m_glide = 0.;
	float *m_glide_mod;

	float *m_pitch_mod_exp;
	float *m_pitch_mod_lin;
	//float *m_vol_mod;
	float *m_pitchbend;

	void setOctaveMultiplier(float p_octave_multiplier) {
		m_octave_multiplier        = p_octave_multiplier;
		m_pitch_control_multiplier = m_octave_multiplier * m_semitone_multiplier * m_cent_multiplier;
	}

	void setSemitoneMultiplier(float p_semitone_multiplier) {
		m_semitone_multiplier      = p_semitone_multiplier;
		m_pitch_control_multiplier = m_octave_multiplier * m_semitone_multiplier * m_cent_multiplier;
	}

	void setCentMultiplier(float p_cent_mutliplier) {
		m_cent_multiplier          = p_cent_mutliplier;
		m_pitch_control_multiplier = m_octave_multiplier * m_semitone_multiplier * m_cent_multiplier;
	}

	void setUnisonDetuneFactorPointer(float* p_pointer){
		m_unison_detune_factor_pointer = p_pointer;
	}

protected:
	float m_octave_multiplier        = 1.f;
	float m_semitone_multiplier      = 1.f;
	float m_cent_multiplier          = 1.f;
	float m_pitch_control_multiplier = 1.f; //all of above multiplied together

	//int m_octave;    // octave tweak
	//int m_semitones; // semitones tweak
	//int m_cent;      // cents tweak

	double m_samplerate = -1; // fs
	double m_one_over_samplerate;
	double m_osc_freq_modded;    // current (actual) frequency of oscillator
	float m_mod_exp_other = 0.f; // can be used for whatever (Chiptune Arp uses it)

	float* m_unison_detune_factor_pointer = nullptr;
	float unison_detune_factor_dummy = 1.f;
};
