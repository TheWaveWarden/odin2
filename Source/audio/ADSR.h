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

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

#include <functional>

#define MIN_DECAY_RELEASE_VAL 0.001

#define ADSR_SECTION_INIT -1
#define ADSR_SECTION_ATTACK 0
#define ADSR_SECTION_DECAY 1
#define ADSR_SECTION_SUSTAIN 2
#define ADSR_SECTION_RELEASE 3
#define ADSR_SECTION_FINISHED 4

#define ATTACK_LOWER_LIMIT 0.000001
#define DECAY_LOWER_LIMIT 0.000001
#define RELEASE_LOWER_LIMIT 0.000001

#include <cmath>

class ADSREnvelope {
public:
	ADSREnvelope();
	~ADSREnvelope();

	float doEnvelope();
	void startRelease();
	int getCurrentSection();

	bool isEnvelopeOff() {
		return (m_current_section == ADSR_SECTION_INIT || m_current_section == ADSR_SECTION_FINISHED);
	}

	// start attack if state is > release, else do nothing
	void restartEnvelopeLegato() {
		m_current_value = m_last_actual_value;
		if (m_current_section == ADSR_SECTION_RELEASE || m_current_section == ADSR_SECTION_FINISHED)
			m_current_section = 0;
	}

	void setEnvelopeOff() {
		m_current_section = 4; // = after release
	}

	inline void reset() {
		m_current_section    = -1;
		m_current_value      = 0.f;
		m_attack_start_value = 0.f;
		m_last_decay         = -1.;
		m_last_release       = -1.;
	}

	bool isBeforeRelease() {
		return (m_current_section < 3 && m_current_section > -1);
	}

	inline void setLoop(bool p_loop) {
		m_loop = p_loop;
	}

	inline void setSampleRate(float p_samplerate) {
		m_samplerate = p_samplerate;
		reset();
		////DBG("setsamplerate adsr");
	}

	inline void setAttack(float p_attack) {
		m_attack = p_attack;
	}

	inline void setDecay(float p_decay) {
		m_decay = p_decay;
	}

	inline void setSustain(float p_sustain) {
		m_sustain = p_sustain;
	}

	inline void setRelease(float p_release) {
		m_release = p_release;
	}

	inline double calcModFactor(double p_mod) {
		return pow(2, 3 * p_mod);
	}

	inline double calcDecayFactor(double p_decay) {
		if (p_decay != m_last_decay) {
			m_last_decay_return = pow(MIN_DECAY_RELEASE_VAL, 1. / (m_samplerate * p_decay));
			m_last_decay        = p_decay;
		}
		return m_last_decay_return;
	}

	inline double calcReleaseFactor(double p_release) {
		if (p_release != m_last_release) {
			m_last_release_return = pow(MIN_DECAY_RELEASE_VAL, 1. / (m_samplerate * p_release));
			m_last_release        = p_release;
		}
		return m_last_release_return;
	}

	// /*inline */ double calcDecayFactorCheap(double p_decay) {
	//   return CheapFunctions::getInstance().cheapADSRPow(m_samplerate * p_decay);
	// }

	void setAttackModPointer(float *p_pointer) {
		m_attack_mod = p_pointer;
	}
	void setDecayModPointer(float *p_pointer) {
		m_decay_mod = p_pointer;
	}
	void setSustainModPointer(float *p_pointer) {
		m_sustain_mod = p_pointer;
	}
	void setReleaseModPointer(float *p_pointer) {
		m_release_mod = p_pointer;
	}

	// used to signal voice endign if this is amp-envelope
	// std::function<void()> onEnvelopeEnd = []() {};
	void onEnvelopeEnd() {
		if (m_test && m_voice_manager_bool_pointer) {
			*m_voice_manager_bool_pointer = false;
			*m_test                       = false;
		}
	}

	//! WEIRD STUFF! 500th workaround for visual studio cant handle my
	//! lambda..........
	void setEnvelopeEndPointers(bool *p_voice, bool *p_manager) {
		m_voice_manager_bool_pointer = p_manager;
		m_test                       = p_voice;
		// todo remove
		*m_voice_manager_bool_pointer = false;
		*m_test                       = false;
	}

protected:
	//! WEIRD STUFF! 500th workaround for visual studio cant handle my
	//! lambda..........
	bool *m_voice_manager_bool_pointer = nullptr;
	//? IF I REMOVE THE FOLLOWING LINE; EVERYTHING GOES DOWN THE FUCK
	bool *m_voice_bool_pointer = nullptr;
	bool *m_test               = nullptr;

	int m_current_section = -1; // before start:-1, A0, D1, S2, R3
	bool m_loop           = false;

	// store values so we dont calculate pow() all the time
	double m_last_decay          = -1.f;
	double m_last_decay_return   = 0.f;
	double m_last_release        = 0.f;
	double m_last_release_return = 0.f;

	float m_last_actual_value = 0.f;

	float *m_attack_mod;
	float *m_decay_mod;
	float *m_sustain_mod;
	float *m_release_mod;

	// these are the initial values for the algorithm as well
	double m_attack  = 0.01f;
	double m_decay   = 1.f;
	double m_sustain = 0.5f;
	double m_release = 0.01f;

	double m_attack_start_value  = 0.f;
	double m_decay_factor        = 0.9998;
	double m_release_factor      = 0.9998;
	double m_release_start_value = 1.;

	double m_current_value = 0.f;
	double m_samplerate    = -1;
};
