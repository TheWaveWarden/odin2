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

#include "../Filters/VAOnePoleFilter.h"

class NoiseOscillator
{
public:
	NoiseOscillator();
	~NoiseOscillator();

	float doNoise();   

	void setFilterFreqs(float p_lowpass_freq, float p_highpass_freq);
	void setHPFreq(float p_freq);
	void setLPFreq(float p_freq);
	// void setVolModPointer(float* p_pointer){
	// 	m_vol_mod = p_pointer;
	// }

	void setSampleRate(float p_samplerate){
  		m_lowpass.setSampleRate(p_samplerate);
  		m_highpass.setSampleRate(p_samplerate);
	}

	void setHPModPointer(float* p_pointer){
		m_highpass.setFreqModPointer(p_pointer);
	}

	void setLPModPointer(float* p_pointer){
		m_lowpass.setFreqModPointer(p_pointer);
	}

	//inline void startOscillator(){
	//	m_note_on = true;
	//}
	
	//inline void stopOscillator(){
	//	m_note_on = false;
	//}

protected:

	//float* m_vol_mod;
	//float* m_lp_mod;
	//float* m_hp_mod;

	VAOnePoleFilter m_lowpass;
	VAOnePoleFilter m_highpass;
};

