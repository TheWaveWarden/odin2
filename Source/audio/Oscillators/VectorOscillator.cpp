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

#include "VectorOscillator.h"

VectorOscillator::VectorOscillator() {
	m_nr_of_wavetables = NUMBER_OF_WAVETABLES + 9; // 9 extra for drawtables
}

VectorOscillator::~VectorOscillator() {
}

void VectorOscillator::selectWavetable(int p_wavetable_index, int p_vector_point) {
	if (p_wavetable_index < m_nr_of_wavetables) {
		m_wavetable_index[p_vector_point] = p_wavetable_index;
	}
}

float VectorOscillator::doOscillate() {
    jassert(m_samplerate > 0);

	return doVectortable();
}

void VectorOscillator::update() {
	Oscillator::update();

	m_wavetable_inc   = WAVETABLE_LENGTH * m_increment;
	m_sub_table_index = getTableIndex();

	for (int i = 0; i < VECTOR_EDGES; ++i) {
		m_current_table_vec[i] = m_wavetable_pointers[m_wavetable_index[i]][m_sub_table_index];
	}
}

float VectorOscillator::doVectortable() {
    jassert(m_samplerate > 0);

	// smooth controls
	m_XY_pad_x_smooth += (m_XY_pad_x - m_XY_pad_x_smooth) * 0.001;
	m_XY_pad_y_smooth += (m_XY_pad_y - m_XY_pad_y_smooth) * 0.001;

	// prepare both sides and interpol value
	int read_index_trunc = (int)m_read_index;
	float fractional     = m_read_index - (float)read_index_trunc;
	int read_index_next  = read_index_trunc + 1 >= WAVETABLE_LENGTH ? 0 : read_index_trunc + 1;

	// do linear interpolation
	float output[VECTOR_EDGES] = {
	    linearInterpolation(
	        m_current_table_vec[0][read_index_trunc], m_current_table_vec[0][read_index_next], fractional),
	    linearInterpolation(
	        m_current_table_vec[1][read_index_trunc], m_current_table_vec[1][read_index_next], fractional),
	    linearInterpolation(
	        m_current_table_vec[2][read_index_trunc], m_current_table_vec[2][read_index_next], fractional),
	    linearInterpolation(
	        m_current_table_vec[3][read_index_trunc], m_current_table_vec[3][read_index_next], fractional)};

	float x_modded = m_XY_pad_x_smooth + *m_mod_x;
	x_modded       = x_modded > 1 ? 1 : x_modded;
	x_modded       = x_modded < 0 ? 0 : x_modded;
	float y_modded = m_XY_pad_y_smooth + *m_mod_y;
	y_modded       = y_modded > 1 ? 1 : y_modded;
	y_modded       = y_modded < 0 ? 0 : y_modded;

	// 1---2
	// |   |
	// 0---3

	// reuse 0 as "lower" and 1 as "upper"
	output[0] = (1.f - x_modded) * output[0] + x_modded * output[3];
	output[1] = (1.f - x_modded) * output[1] + x_modded * output[2];

	m_read_index += m_wavetable_inc * m_sync_anti_aliasing_inc_factor;
	checkWrapIndex(m_read_index);

	// return interpolation between "upper" and "lower"
	return (1.f - y_modded) * output[0] + y_modded * output[1];
}