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

#include "AnalogOscillator.h"
#include <cstdlib>

#define SQUARE_VOLUME_SCALAR 0.3

#define DRIFT_NOISE_SMOOTH_FACTOR 0.0001

AnalogOscillator::AnalogOscillator() {
	m_nr_of_wavetables = 4;
}

AnalogOscillator::~AnalogOscillator() {
}

void AnalogOscillator::loadWavetables() {
	// load the saw table for square as well since PWM is generated from saw
	setWavetablePointer(0, m_WT_container->getWavetablePointers("FatSaw"));
	setWavetablePointer(1, m_WT_container->getWavetablePointers("FatSaw"));
	setWavetablePointer(2, m_WT_container->getWavetablePointers("Triangle"));
	setWavetablePointer(3, m_WT_container->getWavetablePointers("Sine"));

	// init drift generator
	m_drift_generator.initialize(m_samplerate);
}

float AnalogOscillator::generateDrift() {
	return m_drift_generator.doDrift();
}

void AnalogOscillator::update() {
	m_mod_exp_other = generateDrift() * m_drift * DRIFT_RANGE_SEMITONES;
	// DBG(m_mod_exp_other);
	//DBG(m_read_index);
	WavetableOsc1D::update();

}

float AnalogOscillator::doOscillate() {
    jassert(m_samplerate > 0);

	if (m_wavetable_index != 1) {
		return doWavetable();
	}

	return doSquare();
}

float AnalogOscillator::doSquare() {
	// square is done by subtracting two saw waves
	// prepare both sides and interpol value for base and offset value
	m_duty_smooth = m_duty_smooth * PWM_SMOOTHIN_FACTOR + (1 - PWM_SMOOTHIN_FACTOR) * (m_duty);

	float duty_modded = (m_duty_smooth + (*pwm_mod) / 2);

	double read_index_offset = m_read_index + duty_modded * WAVETABLE_LENGTH;
	checkWrapIndex(read_index_offset);

	int read_index_trunc        = (int)m_read_index;
	int read_index_trunc_offset = (int)(read_index_offset);

	float fractional        = m_read_index - (float)read_index_trunc;
	float fractional_offset = read_index_offset - (float)read_index_trunc_offset;

	int read_index_next        = read_index_trunc + 1 >= WAVETABLE_LENGTH ? 0 : read_index_trunc + 1;
	int read_index_next_offset = read_index_trunc_offset + 1 >= WAVETABLE_LENGTH ? 0 : read_index_trunc_offset + 1;

	// do linear interpolation
	float output = linearInterpolation(m_current_table[read_index_trunc], m_current_table[read_index_next], fractional);
	float output_offset = linearInterpolation(
	    m_current_table[read_index_trunc_offset], m_current_table[read_index_next_offset], fractional_offset);

	m_read_index += m_wavetable_inc;
	checkWrapIndex(m_read_index);

	// bring duty modded to range [0,1] for scale calculation
	duty_modded = fmod(duty_modded, 1);

#define MAX(a, b) (a) > (b) ? (a) : (b)

	float square_scale = 2 * (MAX(1 - duty_modded, duty_modded));

	return (output - output_offset) * square_scale * SQUARE_VOLUME_SCALAR;
}
