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

// The code for this filter is based on the diagrams and implementations in Will Pirkles book
// "Designing Software Synthesizer Plug-Ins in C++". Find out more on https://willpirkle.com

#include "LadderFilter.h"

LadderFilter::LadderFilter() {

	// init
	m_k       = 0.0;
	m_alpha_0 = 1.0;
	m_a       = 0.0;
	m_b       = 0.0;
	m_c       = 0.0;
	m_d       = 0.0;

	// set all as LPF types
	m_LPF1.setLP();
	m_LPF2.setLP();
	m_LPF3.setLP();
	m_LPF4.setLP();

	// set default filter type
	m_filter_type = FILTERTYPE::LP4;

	// flush everything
	reset();
}

LadderFilter::~LadderFilter() {
}

void LadderFilter::reset() {
	// flush everything
	m_LPF1.reset();
	m_LPF2.reset();
	m_LPF3.reset();
	m_LPF4.reset();
}

// decode the Q value; Q on UI is 1->10
void LadderFilter::setResControl(double p_res) {
	// this maps dQControl = 0->1 to 0-4 * 0.97 to avoid clippy self oscillation
	m_k                = 3.88 * p_res;
	m_last_freq_modded = -1; //to signal recalculation of coeffs in update()
}
