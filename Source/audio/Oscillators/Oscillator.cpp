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

#include "Oscillator.h"

Oscillator::Oscillator() {
	m_unison_detune_factor_pointer = &unison_detune_factor_dummy;
}

Oscillator::~Oscillator() {
}

float Oscillator::pitchShiftMultiplier(float p_semitones) {
	//note: fastexp was tested and showed a deviation of 0.75 cents in this range
	if (p_semitones < 48.f && p_semitones > -48.f) {
		return juce::dsp::FastMathApproximations::exp(0.05776226504 * p_semitones);
	}
	//0.05776226504 = ln(2)/12
	return std::exp(0.05776226504 * p_semitones);
}

void Oscillator::reset() {
	// modulation variables
	m_mod_freq_exp = 0.0;
	m_mod_freq_lin = 0.0;
}

void Oscillator::setSampleRate(float p_samplerate) {
	m_samplerate          = p_samplerate;
	m_one_over_samplerate = 1. / p_samplerate;
}

void Oscillator::voiceStart(bool p_randomize_phase) {
	if (p_randomize_phase) {
		randomizePhase();
	}
	if (m_reset_active) {
		//DBG("RESET!");
		reset();
	}
}

void Oscillator::setResetActive(bool p_reset) {
	m_reset_active = p_reset;
}

void Oscillator::setGlideTargetFrequency(float p_freq) {
	m_osc_freq_glide_target = p_freq;
}

void Oscillator::setBaseFrequency(float p_freq) {
	m_osc_freq_base = p_freq;
}

void Oscillator::killGlide() {
	m_osc_freq_base = m_osc_freq_glide_target;
}

void Oscillator::setPitchModExpPointer(float *p_pointer) {
	m_pitch_mod_exp = p_pointer;
}

void Oscillator::setPitchModLinPointer(float *p_pointer) {
	m_pitch_mod_lin = p_pointer;
}

void Oscillator::setPitchBendPointer(float *p_pointer) {
	m_pitchbend = p_pointer;
}

void Oscillator::setGlidePointer(float *p_pointer) {
	m_glide_mod = p_pointer;
}

void Oscillator::setGlide(float p_glide) {
	m_glide = p_glide;
}

void Oscillator::setOctaveMultiplier(float p_octave_multiplier) {
	m_octave_multiplier        = p_octave_multiplier;
	m_pitch_control_multiplier = m_octave_multiplier * m_semitone_multiplier * m_cent_multiplier;
}

void Oscillator::setSemitoneMultiplier(float p_semitone_multiplier) {
	m_semitone_multiplier      = p_semitone_multiplier;
	m_pitch_control_multiplier = m_octave_multiplier * m_semitone_multiplier * m_cent_multiplier;
}

void Oscillator::setCentMultiplier(float p_cent_mutliplier) {
	m_cent_multiplier          = p_cent_mutliplier;
	m_pitch_control_multiplier = m_octave_multiplier * m_semitone_multiplier * m_cent_multiplier;
}

void Oscillator::setUnisonDetuneFactorPointer(float *p_pointer) {
	m_unison_detune_factor_pointer = p_pointer;
}