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

#include "BiquadFilter.h"

BiquadFilter::BiquadFilter() {
}

BiquadFilter::~BiquadFilter() {
}

void BiquadFilter::setFilterCoefficients(double p_a1, double p_a2, double p_b0, double p_b1, double p_b2) {
	m_a1 = p_a1;
	m_a2 = p_a2;
	m_b0 = p_b0;
	m_b1 = p_b1;
	m_b2 = p_b2;
}

double BiquadFilter::doFilter(double p_input) {
    jassert(m_samplerate > 0);

	//this uses direct form 2 from wikipedia page
	//https://en.wikipedia.org/wiki/Digital_biquad_filter
	double left_side = p_input - m_z2 * m_a2 - m_z1 * m_a1;
	double output    = m_b2 * m_z2 + m_b1 * m_z1 + m_b0 * left_side;

	//update buffers
	m_z2 = m_z1;
	m_z1 = left_side;

	return output;
}
