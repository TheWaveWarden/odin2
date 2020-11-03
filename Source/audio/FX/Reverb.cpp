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

float clamp(float min, float val, float max) {
	if (val < min)
		return min;
	else if (val > max)
		return max;
	return val;
}

// enum revparam {
// 	r2p_predelay = 0,

// 	r2p_room_size,
// 	r2p_decay_time,
// 	r2p_diffusion,
// 	r2p_buildup,
// 	r2p_modulation,

// 	r2p_lf_damping,
// 	r2p_hf_damping,

// 	r2p_width,
// 	r2p_mix,
// 	r2p_num_params,
// };

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

Reverb2Effect::Reverb2Effect(/*SurgeStorage* storage, FxStorage* fxdata, pdata* pd*/)
/*: Effect(storage, fxdata, pd)*/
{
	m_state = 0.f;
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

void Reverb2Effect::update_rtime() {
	float t = BLOCK_SIZE_INV *
	          (m_samplerate * (std::max(1.0f, powf(2.f, m_decay_time)) * 2.f +
	                           std::max(0.1f, powf(2.f, m_predelay_val)) * 2.f)); // *2 is to get the db120 time
	ringout_time = (int)t;
}

void Reverb2Effect::setRoomSize(float p_room_size) {
	m_scale = powf(2.f, 1.f * p_room_size);
	calc_size(m_scale);
}

void Reverb2Effect::setDecayTime(float p_decay_time) {

	if (fabs(p_decay_time - m_last_decay_time) > 0.001f) {
		update_rtime();
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
	mix = p_mix;
}

void Reverb2Effect::setPreDelay(float p_predelay) {
	m_pre_delay_time = clamp(1, (int)(m_samplerate * pow(2.0, p_predelay) * 1.f), PREDELAY_BUFFER_SIZE_LIMIT - 1);
}

//todo is static?
//m_lfo.set_rate(2.0 * M_PI * /*powf(2, -2.f)*/ 0.25 * dsamplerate_inv);

void Reverb2Effect::process(float &dataL, float &dataR) {

	float wetL, wetR;

	//for (int k = 0; k < BLOCK_SIZE; k++) {
	float in = (dataL + dataR) * 0.5f;

	in = m_predelay.process(in, m_pre_delay_time);

	in      = m_input_allpass[0].process(in, m_diffusion);
	in      = m_input_allpass[1].process(in, m_diffusion);
	in      = m_input_allpass[2].process(in, m_diffusion);
	in      = m_input_allpass[3].process(in, m_diffusion);
	float x = m_state;

	float outL = 0.f;
	float outR = 0.f;

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

		x = m_hf_damper[b].process_lowpass(x, hdc);
		x = m_lf_damper[b].process_highpass(x, ldc);

		int modulation = (int)(m_modulation * lfos[b] * (float)DELAY_SUBSAMPLE_RANGE);
		float tap_outL = 0.f;
		float tap_outR = 0.f;
		x              = m_delay[b].process(x, m_tap_timeL[b], tap_outL, m_tap_timeR[b], tap_outR, modulation);
		outL += tap_outL * m_tap_gainL[b];
		outR += tap_outR * m_tap_gainR[b];

		x *= m_decay_multiply;
	}

	wetL    = outL;
	wetR    = outR;
	m_state = x;
	//}

	// scale width
	//float M[BLOCK_SIZE], S[BLOCK_SIZE];
	//encodeMS(wetL, wetR, M, S, BLOCK_SIZE_QUAD);
	//width.multiply_block(S, BLOCK_SIZE_QUAD);
	//decodeMS(M, S, wetL, wetR, BLOCK_SIZE_QUAD);
	//todo encode mid-side, scale width.... width is buffer??

	//mix.fade_2_blocks_to(dataL, wetL, dataR, wetR, dataL, dataR, BLOCK_SIZE_QUAD);
	//todo output signal
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
	//Effect::init_ctrltypes();

	// m_fxdata_predelay.set_name("Pre-Delay");
	// m_fxdata_predelay.set_type(ct_reverbpredelaytime);

	// m_fxdata_room_size.set_name("Room Size");
	// m_fxdata_room_size.set_type(ct_percent_bidirectional);
	// m_fxdata_decay_time.set_name("Decay Time");
	// m_fxdata_decay_time.set_type(ct_reverbtime);
	// m_fxdata_diffusion.set_name("Diffusion");
	// m_fxdata_diffusion.set_type(ct_percent);
	// m_fxdata_buildup.set_name("Buildup");
	// m_fxdata_buildup.set_type(ct_percent);
	// m_fxdata_modulation.set_name("Modulation");
	// m_fxdata_modulation.set_type(ct_percent);

	// m_fxdata_hf_damping.set_name("HF Damping");
	// m_fxdata_hf_damping.set_type(ct_percent);
	// m_fxdata_lf_damping.set_name("LF Damping");
	// m_fxdata_lf_damping.set_type(ct_percent);

	// m_fxdata_width.set_name("Width");
	// m_fxdata_width.set_type(ct_decibel_narrow);
	// m_fxdata_mix.set_name("Mix");
	// m_fxdata_mix.set_type(ct_percent);

	// for( int i=r2p_predelay; i<r2p_num_params; ++i )
	// {
	//    auto a = 1;
	//    if( i >= r2p_room_size ) a += 2;
	//    if( i >= r2p_lf_damping ) a += 2;
	//    if( i >= r2p_width ) a += 2;
	//    fxdata->p[i].posy_offset = a;
	// }
}

void Reverb2Effect::init_default_values() {
	m_fxdata_predelay   = -4.f;
	m_fxdata_decay_time = 0.75f;
	m_fxdata_mix        = 0.33f;
	m_fxdata_width      = 0.0f;
	m_fxdata_diffusion  = 1.0f;
	m_fxdata_buildup    = 1.0f;
	m_fxdata_modulation = 0.5f;
	m_fxdata_hf_damping = 0.2f;
	m_fxdata_lf_damping = 0.2f;
	m_fxdata_room_size  = 0.f;
}

void Reverb2Effect::setSampleRate(float p_sr) {
	m_samplerate    = p_sr;
	dsamplerate_inv = 1. / (double)p_sr;
}
