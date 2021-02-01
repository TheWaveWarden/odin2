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

#include "DriftGenerator.h"
#include "../../GlobalIncludes.h"
#include "stdlib.h"

DriftGenerator::DriftGenerator() {
}

DriftGenerator::~DriftGenerator() {
}

void DriftGenerator::initialize(float p_samlerate) {

	m_samplerate   = p_samlerate;
	m_drift_length = (int)(DRIFT_LENGTH_SECONDS * p_samlerate);

	m_last_value = getRand();
	m_next_value = getRand();
}

float DriftGenerator::doDrift() {
    jassert(m_samplerate > 0);

	if (++m_counter > m_drift_length) {
		m_counter = 0;
		calcNewCoeffs();
	}

	return (m_next_value - m_last_value) * (float)m_counter / m_drift_length + m_last_value;
}

void DriftGenerator::setSampleRate(float p_samplerate) {
	m_samplerate          = p_samplerate;
	m_one_over_samplerate = 1. / p_samplerate;
	m_drift_length        = (int)(DRIFT_LENGTH_SECONDS * p_samplerate);
}

void DriftGenerator::calcNewCoeffs() {
	m_last_value = m_next_value;
	m_next_value = getRand();
}

float DriftGenerator::getRand() {
	float random = (float)rand();
	return 2.f * ((float)random / (float)RAND_MAX) - 1.f;
}
