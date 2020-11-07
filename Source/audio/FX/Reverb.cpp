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

// The implementation for this ZitaReverb module is taken from zita-rev1 with some adjustments. See
// https://github.com/royvegard/zita-rev1 for the original code.

#include "Reverb.h"
#include <algorithm>

/* reverb 2			*/
/* allpass loop design */

float db_to_linear(float x) {
	return juce::Decibels::decibelsToGain(x);
}

#define clamp(min, val, max) ((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))

const float db60 = powf(10.f, 0.05f * -60.f);

Reverb2Effect::allpass::allpass() {
	m_k   = 0;
	m_len = 1;
	memset(m_data, 0, MAX_ALLPASS_LEN * sizeof(float));
}

void Reverb2Effect::allpass::setLen(int len) {
	m_len = len;
}

float Reverb2Effect::allpass::process(float in, float coeff) {
	m_k++;
	if (m_k >= m_len)
		m_k = 0;
	float delay_in = in - coeff * m_data[m_k];
	float result   = m_data[m_k] + coeff * delay_in;
	m_data[m_k]    = delay_in;
	return result;
}

Reverb2Effect::delay::delay() {
	m_k   = 0;
	m_len = 1;
	memset(m_data, 0, MAX_DELAY_LEN * sizeof(float));
}

void Reverb2Effect::delay::setLen(int len) {
	m_len = len;
}

float Reverb2Effect::delay::process(float in, int tap1, float &tap_out1, int tap2, float &tap_out2, int modulation) {
	m_k = (m_k + 1) & DELAY_LEN_MASK;

	tap_out1 = m_data[(m_k - tap1) & DELAY_LEN_MASK];
	tap_out2 = m_data[(m_k - tap2) & DELAY_LEN_MASK];

	int modulation_int   = modulation >> DELAY_SUBSAMPLE_BITS;
	int modulation_frac1 = modulation & (DELAY_SUBSAMPLE_RANGE - 1);
	int modulation_frac2 = DELAY_SUBSAMPLE_RANGE - modulation_frac1;

	float d1               = m_data[(m_k - m_len + modulation_int + 1) & DELAY_LEN_MASK];
	float d2               = m_data[(m_k - m_len + modulation_int) & DELAY_LEN_MASK];
	const float multiplier = 1.f / (float)(DELAY_SUBSAMPLE_RANGE);

	float result = (d1 * (float)modulation_frac1 + d2 * (float)modulation_frac2) * multiplier;
	m_data[m_k]  = in;

	return result;
}

Reverb2Effect::onepole_filter::onepole_filter() {
	m_a0 = 0.f;
}

float Reverb2Effect::onepole_filter::process_lowpass(float x, float c0) {
	m_a0 = m_a0 * c0 + x * (1.f - c0);
	return m_a0;
}

float Reverb2Effect::onepole_filter::process_highpass(float x, float c0) {
	m_a0 = m_a0 * (1.f - c0) + x * c0;
	return x - m_a0;
}

Reverb2Effect::Reverb2Effect() {
	m_state = 0.f;
	init_default_values();
}

Reverb2Effect::~Reverb2Effect() {
}

void Reverb2Effect::init() {
	setvars(true);
}

int Reverb2Effect::msToSamples(float ms, float scale) {
	float a = m_samplerate * ms * 0.001f;

	float b = a * scale;

	return (int)(b);
}

void Reverb2Effect::calc_size(float scale) {
	float m = scale;

	m_tap_timeL[0] = msToSamples(80.3, m);
	m_tap_timeL[1] = msToSamples(59.3, m);
	m_tap_timeL[2] = msToSamples(97.7, m);
	m_tap_timeL[3] = msToSamples(122.6, m);
	m_tap_timeR[0] = msToSamples(35.5, m);
	m_tap_timeR[1] = msToSamples(101.6, m);
	m_tap_timeR[2] = msToSamples(73.9, m);
	m_tap_timeR[3] = msToSamples(80.3, m);

	m_input_allpass[0].setLen(msToSamples(4.76, m));
	m_input_allpass[1].setLen(msToSamples(6.81, m));
	m_input_allpass[2].setLen(msToSamples(10.13, m));
	m_input_allpass[3].setLen(msToSamples(16.72, m));

	m_allpass[0][0].setLen(msToSamples(38.2, m));
	m_allpass[0][1].setLen(msToSamples(53.4, m));
	m_delay[0].setLen(msToSamples(178.8, m));

	m_allpass[1][0].setLen(msToSamples(44.0, m));
	m_allpass[1][1].setLen(msToSamples(41, m));
	m_delay[1].setLen(msToSamples(126.5, m));

	m_allpass[2][0].setLen(msToSamples(48.3, m));
	m_allpass[2][1].setLen(msToSamples(60.5, m));
	m_delay[2].setLen(msToSamples(106.1, m));

	m_allpass[3][0].setLen(msToSamples(38.9, m));
	m_allpass[3][1].setLen(msToSamples(42.2, m));
	m_delay[3].setLen(msToSamples(139.4, m));
}

void Reverb2Effect::setvars(bool init) {
	// TODO, balance the gains from the calculated decay coefficient?

	m_tap_gainL[0] = 1.5f / 4.f;
	m_tap_gainL[1] = 1.2f / 4.f;
	m_tap_gainL[2] = 1.0f / 4.f;
	m_tap_gainL[3] = 0.8f / 4.f;
	m_tap_gainR[0] = 1.5f / 4.f;
	m_tap_gainR[1] = 1.2f / 4.f;
	m_tap_gainR[2] = 1.0f / 4.f;
	m_tap_gainR[3] = 0.8f / 4.f;

	calc_size(1.f);
}

void Reverb2Effect::update_ringout_time() {
	float t = BLOCK_SIZE_INV *
	          (m_samplerate * (std::max(1.0f, powf(2.f, m_decay_time)) * 2.f +
	                           std::max(0.1f, powf(2.f, m_predelay_val)) * 2.f)); // *2 is to get the db120 time
	ringout_time = (int)t;
}

void Reverb2Effect::setRoomSize(float p_room_size) {
	m_scale = powf(2.f, p_room_size);
	calc_size(m_scale);
}

void Reverb2Effect::setDecayTime(float p_decay_time) {

	if (fabs(p_decay_time - m_last_decay_time) > 0.001f) {
		update_ringout_time();
		m_last_decay_time = p_decay_time;
	}

	float loop_time_s = 0.5508 * m_scale;
	float decay       = powf(db60, loop_time_s / (4.f * (powf(2.f, m_decay_time))));
	m_decay_multiply  = decay;
}

void Reverb2Effect::setDiffusion(float p_diffusion) {
	m_diffusion = 0.7f * p_diffusion;
}

void Reverb2Effect::setBuildup(float p_buildup) {
	m_buildup = 0.7f * p_buildup;
}

void Reverb2Effect::setHFDamp(float p_hf_damping) {
	m_hf_damp_coefficent = 0.8 * p_hf_damping;
}

void Reverb2Effect::setLFDamp(float p_lf_damping) {
	m_lf_damp_coefficent = 0.2 * p_lf_damping;
}

void Reverb2Effect::setWidth(float p_width) {
	m_width = db_to_linear(p_width);
}

void Reverb2Effect::setMix(float p_mix) {
	m_mix = p_mix;
}

void Reverb2Effect::setPreDelayMs(float p_predelay) {
	m_pre_delay_time = clamp(1, (int)(m_samplerate * p_predelay * 0.001), PREDELAY_BUFFER_SIZE_LIMIT - 1);
}

void Reverb2Effect::setModulation(float p_modulation){
	m_modulation = p_modulation;
}


void Reverb2Effect::process(float &dataL, float &dataR) {

	//for (int k = 0; k < BLOCK_SIZE; k++) {
	float in = (dataL + dataR) * 0.5f;

	in = m_predelay.process(in, m_pre_delay_time);

	in      = m_input_allpass[0].process(in, m_diffusion);
	in      = m_input_allpass[1].process(in, m_diffusion);
	in      = m_input_allpass[2].process(in, m_diffusion);
	in      = m_input_allpass[3].process(in, m_diffusion);
	float x = m_state;

	float wetL = 0.f;
	float wetR = 0.f;

	float lfos[NUM_BLOCKS];
	lfos[0] = m_lfo.r;
	lfos[1] = m_lfo.i;
	lfos[2] = -m_lfo.r;
	lfos[3] = -m_lfo.i;

	auto hdc = clamp(0.01f, m_hf_damp_coefficent, 0.99f);
	auto ldc = clamp(0.01f, m_lf_damp_coefficent, 0.99f);

	for (int b = 0; b < NUM_BLOCKS; b++) {
		x = x + in;

		for (int c = 0; c < NUM_ALLPASSES_PER_BLOCK; c++) {
			x = m_allpass[b][c].process(x, m_buildup);
		}

		//HF / LF Damping filters
		x = m_hf_damper[b].process_lowpass(x, hdc);
		x = m_lf_damper[b].process_highpass(x, ldc);

		int modulation = (int)(m_modulation * lfos[b] * (float)DELAY_SUBSAMPLE_RANGE);
		float tap_outL = 0.f;
		float tap_outR = 0.f;
		x              = m_delay[b].process(x, m_tap_timeL[b], tap_outL, m_tap_timeR[b], tap_outR, modulation);
		wetL += tap_outL * m_tap_gainL[b];
		wetR += tap_outR * m_tap_gainR[b];

		x *= m_decay_multiply;
	}

	m_state = x;
	//}


	float mid  = wetL + wetR;
	float side = wetL - wetR;
	side *= m_width;
	wetL = (mid + side) * 0.5;
	wetR = (mid - side) * 0.5;

	//mix.fade_2_blocks_to(dataL, wetL, dataR, wetR, dataL, dataR, BLOCK_SIZE_QUAD);
	dataL = (1.f - m_mix) * dataL + m_mix * wetL;
	dataR = (1.f - m_mix) * dataR + m_mix * wetR;
}

void Reverb2Effect::suspend() {
	init();
}

const char *Reverb2Effect::group_label(int id) {
	switch (id) {
	case 0:
		return "Pre-Delay";
	case 1:
		return "Reverb";
	case 2:
		return "EQ";
	case 3:
		return "Output";
	}
	return 0;
}
int Reverb2Effect::group_label_ypos(int id) {
	switch (id) {
	case 0:
		return 1;
	case 1:
		return 5;
	case 2:
		return 17;
	case 3:
		return 23;
	}
	return 0;
}

void Reverb2Effect::init_ctrltypes() {
	//todo remove?
}

void Reverb2Effect::init_default_values() {
	setPreDelayMs(10.f);
	setDecayTime(0.75f);
	setMix(0.5);
	setWidth(0.0f);
	setDiffusion(1.f);
	setBuildup(1.0f);
	setModulation(0.5f);
	setHFDamp(0.2f);
	setLFDamp(0.2f);
	setRoomSize(0.f);
}

void Reverb2Effect::setSampleRate(float p_sr) {
	m_samplerate    = p_sr;
	dsamplerate_inv = 1. / (double)p_sr;

	m_lfo.set_rate(2.0 * M_PI * /*powf(2, -2.f)*/ 0.25 * dsamplerate_inv);

	//todo this should not happen here...
	init_default_values();
	init();
}
