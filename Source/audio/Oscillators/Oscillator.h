/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 - 2021 TheWaveWarden
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

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "../../GlobalIncludes.h"
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
	Oscillator();
	virtual ~Oscillator();

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

	inline double calculateGlide(float p_glide) {
		if (p_glide < 0.01) {
			return 0.;
		} else {
			return 0.9985f + p_glide * 0.0014;
		}
	}

	static float pitchShiftMultiplier(float p_semitones);
	virtual float doOscillate() = 0;
	virtual void randomizePhase() = 0;
	virtual void setSampleRate(float p_samplerate);
	virtual void setGlideTargetFrequency(float p_freq);
	virtual void setBaseFrequency(float p_freq);
	virtual void reset();
	// moves the freq to glide target on osc change, otherwise after oscillator
	// change the oscs glide again
	void killGlide();
	void voiceStart(bool p_randomize_phase);
	void setResetActive(bool p_reset);
	void setPitchModExpPointer(float *p_pointer);
	void setPitchModLinPointer(float *p_pointer);
	void setPitchBendPointer(float *p_pointer);
	void setGlidePointer(float *p_pointer);
	void setGlide(float p_glide);
	void setOctaveMultiplier(float p_octave_multiplier);
	void setSemitoneMultiplier(float p_semitone_multiplier);
	void setCentMultiplier(float p_cent_mutliplier);
	void setUnisonDetuneFactorPointer(float *p_pointer);

	bool m_reset_flag      = false; // this is used by other oscs to know when to sync
	float m_reset_position = 0.f;   //this is used by other oscs to know which pos to sync to
	bool m_reset_active    = false;
	double m_osc_freq_glide_target; // the target freq after glide from MIDI note
	double m_osc_freq_base;         // oscillator frequency which glides towards glide target
	double m_increment;             // phase inc = fo/fs
	double m_mod_freq_exp = 0;      // exp freq modulation input
	double m_mod_freq_lin = 0;      // lin freq modulation input
	float m_glide         = 0.;
	float *m_glide_mod;
	float *m_pitch_mod_exp;
	float *m_pitch_mod_lin;
	float *m_pitchbend;

protected:
	float m_octave_multiplier        = 1.f;
	float m_semitone_multiplier      = 1.f;
	float m_cent_multiplier          = 1.f;
	float m_pitch_control_multiplier = 1.f; //all of above multiplied together
	double m_samplerate              = -1;  // fs
	double m_one_over_samplerate;
	double m_osc_freq_modded;    // current (actual) frequency of oscillator
	float m_mod_exp_other = 0.f; // can be used for whatever (Chiptune Arp uses it)

	float *m_unison_detune_factor_pointer = nullptr;
	float unison_detune_factor_dummy      = 1.f;
};
