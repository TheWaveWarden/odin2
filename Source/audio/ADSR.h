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

#include "JuceHeader.h"
#include "../GlobalIncludes.h"
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
	bool isEnvelopeOff();
	void restartEnvelopeLegato();
	void restartEnvelopeRetrig();
	void setEnvelopeOff();
	void reset();
	bool isBeforeRelease();
	void setLoop(bool p_loop);
	void setSampleRate(float p_samplerate);
	void setAttack(float p_attack);
	void setDecay(float p_decay);
	void setSustain(float p_sustain);
	void setRelease(float p_release);
	double calcModFactor(double p_mod);
	double calcDecayFactor(double p_decay);
	double calcReleaseFactor(double p_release);
	void setAttackModPointer(float *p_pointer);
	void setDecayModPointer(float *p_pointer);
	void setSustainModPointer(float *p_pointer);
	void setReleaseModPointer(float *p_pointer);
	void onEnvelopeEnd();
	void setEnvelopeEndPointers(bool *p_voice, bool *p_manager);

protected:
	bool *m_voice_manager_bool_pointer = nullptr;
	bool *m_voice_end_pointer          = nullptr;

	int m_current_section = ADSR_SECTION_INIT; // before start:-1, A=0, D=1, S=2, R=3, finished=4
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
