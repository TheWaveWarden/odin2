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

#include "Filter.h"

// --- construction
Filter::Filter() {
	// --- defaults
	m_res_base    = 1.0; // Q is 1 to 10 on GUI
	m_freq_modded = FILTER_FC_DEFAULT;
	m_res_modded  = FILTER_Q_DEFAULT;
	m_freq_base   = FILTER_FC_DEFAULT;
	// --- clear
	m_mod_frequency = 0.0;
}

Filter::~Filter() {
}

// --- flush buffers
void Filter::reset() {
}
