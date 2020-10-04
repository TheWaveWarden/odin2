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

#include "NoiseOscillator.h"
//#include "../JuceLibraryCode/JuceHeader.h"
#include "stdlib.h"

NoiseOscillator::NoiseOscillator() {
	m_lowpass.m_freq_base = FILTER_FC_MAX;
	m_lowpass.setLP();

	m_highpass.m_freq_base = FILTER_FC_MIN;
	m_highpass.setHP();

	m_lowpass.reset();
	m_highpass.reset();
}

NoiseOscillator::~NoiseOscillator() {
}

float NoiseOscillator::doNoise() {
	m_lowpass.update();
	m_highpass.update();

	float white_noise = (float)rand();
	white_noise       = 2 * (white_noise / RAND_MAX) - 1;

	// do 2nd order like this?
	white_noise = m_lowpass.doFilter(white_noise);
	white_noise = m_highpass.doFilter(white_noise);

	return white_noise;
}

void NoiseOscillator::setFilterFreqs(float p_lowpass_freq, float p_highpass_freq) {
	m_lowpass.m_freq_base  = p_lowpass_freq;
	m_highpass.m_freq_base = p_highpass_freq;
}

void NoiseOscillator::setHPFreq(float p_freq) {
	m_highpass.m_freq_base = p_freq;
}

void NoiseOscillator::setLPFreq(float p_freq) {
	m_lowpass.m_freq_base = p_freq;
}
