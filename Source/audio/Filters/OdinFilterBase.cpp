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

#include "OdinFilterBase.h"

// --- construction
OdinFilterBase::OdinFilterBase() {
}

OdinFilterBase::~OdinFilterBase() {
}

// --- flush buffers
void OdinFilterBase::reset() {
}

void OdinFilterBase::setFreqModPointer(float *p_pointer) {
	m_freq_mod = p_pointer;
}

void OdinFilterBase::setResModPointer(float *p_pointer) {
	m_res_mod = p_pointer;
}

void OdinFilterBase::setVelModPointer(float *p_pointer) {
	m_vel_mod_mod = p_pointer;
}

void OdinFilterBase::setKbdModPointer(float *p_pointer) {
	m_kbd_mod_mod = p_pointer;
}

void OdinFilterBase::setSaturationModPointer(float *p_pointer) {
	m_saturation_mod = p_pointer;
}

void OdinFilterBase::setEnvModPointer(float *p_pointer) {
	m_env_mod_mod = p_pointer;
}

void OdinFilterBase::setResControl(double p_res) {
}

void OdinFilterBase::setSampleRate(double d) {
	m_samplerate          = d;
	m_one_over_samplerate = 1.f / d;
}