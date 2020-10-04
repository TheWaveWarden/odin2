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

#include "CombFilter.h"

CombFilter::CombFilter() {
}

CombFilter::~CombFilter() {
}

float CombFilter::doFilter(float p_input) {
    jassert(m_samplerate > 0);

	
	if(m_reset_smoothing){
		m_delay_time_smooth = m_delay_time_control;
		m_reset_smoothing = false;
	}

	m_delay_time_smooth = (m_delay_time_smooth - m_delay_time_control) * 0.999 + m_delay_time_control;

	float delay_time_modded = m_delay_time_smooth;
	if (*m_freq_mod + m_vel_mod_amount + *m_vel_mod_mod + m_env_mod_amount + *m_env_mod_mod ||
	    m_kbd_mod_amount + *m_kbd_mod_mod) {
		delay_time_modded *=
		    pitchShiftMultiplier(((-*m_freq_mod) - m_env_value * (m_env_mod_amount + *m_env_mod_mod) -
		                          (m_vel_mod_amount + *m_vel_mod_mod) * (float)m_MIDI_velocity / 127.f) *
		                             48 -
		                         m_MIDI_note * (m_kbd_mod_amount + *m_kbd_mod_mod));
	}
	delay_time_modded = delay_time_modded > 1.f / (float)COMB_FC_MIN ? 1.f / (float)COMB_FC_MIN : delay_time_modded;

	// calc read index
	float read_index     = (float)m_write_index - delay_time_modded * m_samplerate;
	int read_index_trunc = floor(read_index);
	int read_index_next  = read_index_trunc + 1;
	float frac           = read_index - (float)read_index_trunc;

	while (read_index_trunc < 0) {
		read_index_trunc = read_index_trunc + COMB_BUFFER_LENGTH;
	}
	while(read_index_next < 0) {
		read_index_next = read_index_next + COMB_BUFFER_LENGTH;
	}
	while(read_index_next >= COMB_BUFFER_LENGTH) {
		read_index_next = read_index_next - COMB_BUFFER_LENGTH;
	}

	float output = linearInterpolation(circular_buffer[read_index_trunc], circular_buffer[read_index_next], frac);

	float feedback_modded = m_feedback + *m_res_mod;
	feedback_modded       = feedback_modded > m_feedback_higher_limit ? m_feedback_higher_limit : feedback_modded;
	feedback_modded       = feedback_modded < m_feedback_lower_limit ? m_feedback_lower_limit : feedback_modded;

	circular_buffer[m_write_index] = p_input + output * feedback_modded * m_positive_comb;
	incWriteIndex();

	float ret = (p_input + output) * 0.5f;
	m_DC_blocking_filter.doFilter(ret);
	return ret;
}
