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

#define DRIFT_LENGTH_SECONDS 5

class DriftGenerator
{
public:
	DriftGenerator();
	~DriftGenerator();


	float doDrift();
	void initialize(float p_samplerate);
	void setSampleRate(float p_samplerate);
protected:

	float getRand();
	void calcNewCoeffs();

	int m_counter = 0;
	int m_drift_length; // multiples of samplerate
	float m_last_value;
	float m_next_value;
	//float m_a = 0.f;
	//float m_b = 0.f;
	//float m_c = 0.f;

	float m_samplerate = -1;
	float m_one_over_samplerate;
};

