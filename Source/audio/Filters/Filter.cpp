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
}

Filter::~Filter() {
}

// --- flush buffers
void Filter::reset() {
}

void Filter::setFreqModPointer(float *p_pointer) {
	m_freq_mod = p_pointer;
}

void Filter::setResModPointer(float *p_pointer) {
	m_res_mod = p_pointer;
}

void Filter::setVelModPointer(float *p_pointer) {
	m_vel_mod_mod = p_pointer;
}

void Filter::setKbdModPointer(float *p_pointer) {
	m_kbd_mod_mod = p_pointer;
}

void Filter::setSaturationModPointer(float *p_pointer) {
	m_saturation_mod = p_pointer;
}

void Filter::setEnvModPointer(float *p_pointer) {
	m_env_mod_mod = p_pointer;
}

void Filter::setResControl(double p_res) {
}

void Filter::setSampleRate(double d) {
	m_samplerate          = d;
	m_one_over_samplerate = 1.f / d;
}