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

#pragma once


#include <cmath>
#include <cstring>
#include "../../GlobalIncludes.h"
#include "../Filters/BiquadEQ.h"

#define BLOCK_SIZE 1
#define BLOCK_SIZE_INV 1
#define BLOCK_SIZE_QUAD 1

class quadr_osc
{
public:
   quadr_osc()
   {
      r = 0;
      i = -1;
   }
   inline void set_rate(float w)
   {
      dr = cos(w);
      di = sin(w);
      // normalize vector
      double n = 1 / sqrt(r * r + i * i);
      r *= n;
      i *= n;
   }
   inline void set_phase(float w)
   {
      r = sin(w);
      i = -cos(w);
   }
   inline void process()
   {
      float lr = r, li = i;
      r = dr * lr - di * li;
      i = dr * li + di * lr;
   }

public:
   float r, i;

private:
   float dr, di;
};

class SurgeReverb {
	static const int NUM_BLOCKS = 4, NUM_INPUT_ALLPASSES = 4, NUM_ALLPASSES_PER_BLOCK = 2, MAX_ALLPASS_LEN = 16384,
	                 MAX_DELAY_LEN = 16384, DELAY_LEN_MASK = MAX_DELAY_LEN - 1, DELAY_SUBSAMPLE_BITS = 8,
	                 DELAY_SUBSAMPLE_RANGE = (1 << DELAY_SUBSAMPLE_BITS),
	                 PREDELAY_BUFFER_SIZE  = 48000 * 4 * 4, // max sample rate is 48000 * 4 probably
	    PREDELAY_BUFFER_SIZE_LIMIT         = 48000 * 4 * 3; // allow for one second of diffusion

	class allpass {
	public:
		allpass();
		float process(float x, float coeff);
		void setLen(int len);

	private:
		int m_len;
		int m_k;
		float m_data[MAX_ALLPASS_LEN];
	};

	class delay {
	public:
		delay();
		float process(float x, int tap1, float &tap_out1, int tap2, float &tap_out2, int modulation);
		void setLen(int len);

	private:
		int m_len;
		int m_k;
		float m_data[MAX_DELAY_LEN];
	};

	class predelay {
	public:
		predelay() {
			memset(m_data, 0, PREDELAY_BUFFER_SIZE * sizeof(float));
		}
		float process(float in, int tap) {
			m_k = (m_k + 1);
			if (m_k == PREDELAY_BUFFER_SIZE)
				m_k = 0;
			auto p = m_k - tap;
			while (p < 0)
				p += PREDELAY_BUFFER_SIZE;
			auto res = m_data[p];
			m_data[m_k] = in;
			return res;
		}

	private:
		int m_k = 0;
		float m_data[PREDELAY_BUFFER_SIZE];
	};

	class onepole_filter {
	public:
		onepole_filter();
		float process_lowpass(float x, float c0);
		float process_highpass(float x, float c0);

	private:
		float m_a0;
	};

	//lipol_ps mix alignas(16), width alignas(16);
   //float width;

public:
	SurgeReverb(/*SurgeStorage *storage, FxStorage *fxdata, pdata *pd*/);
	~SurgeReverb();
	const char *get_effectname() {
		return "reverb2";
	}
	void init();
	void process(float &dataL, float &dataR);
	void suspend();
	void setvars(bool init);
	void calc_size(float scale);
	void init_ctrltypes();
	void init_default_values();
	const char *group_label(int id);
	int group_label_ypos(int id);
	int get_ringout_decay() {
		return ringout_time;
	}
	int msToSamples(float ms, float scale);
	void setSampleRate(float p_sr);

	void setRoomSize(float p_room_size);
	void setDecayTime(float p_decay_time);
	void setDiffusion(float p_diffusion);
	void setBuildup(float p_buildup);
	//void setHFDamp(float p_hf_damping);
	//void setLFDamp(float p_lf_damping);	
	void setEQGain(float p_gain);
	void setEQQ(float p_Q);
	void setEQFreq(float p_freq);
	void setWidth(float p_width);
	void setMix(float p_mix);
	void setPreDelayMs(float p_predelay);
	void setModulation(float p_modulation);
	void setDucking(float p_ducking);
private:
	float m_samplerate = -1;
	double dsamplerate_inv = -1;

	void update_ringout_time();
	int ringout_time;

	allpass m_input_allpass[NUM_INPUT_ALLPASSES];
	allpass m_allpass[NUM_BLOCKS][NUM_ALLPASSES_PER_BLOCK];
	onepole_filter m_hf_damper[NUM_BLOCKS];
	onepole_filter m_lf_damper[NUM_BLOCKS];
	delay m_delay[NUM_BLOCKS];
	predelay m_predelay;
	quadr_osc m_lfo;
	BiquadEQ m_EQ[2];

	int m_tap_timeL[NUM_BLOCKS];
	int m_tap_timeR[NUM_BLOCKS];
	float m_tap_gainL[NUM_BLOCKS];
	float m_tap_gainR[NUM_BLOCKS];
	float m_state;
	float m_decay_multiply;
	float m_diffusion;
	float m_buildup;
	float m_hf_damp_coefficent;
	float m_lf_damp_coefficent;
	float m_modulation;
	float m_last_decay_time = -1.0;
	float m_decay_time;
	float m_predelay_val;
	float m_width;
	float m_mix;
	float m_roomsize;
	float m_scale;
	int m_pre_delay_time;
	float m_ducking;
};
