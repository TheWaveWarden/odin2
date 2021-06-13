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

#pragma once
#include "WavetableOsc1D.h"
#include "DriftGenerator.h"

#define DRIFT_RANGE_SEMITONES 0.2f;

class AnalogOscillator :
	public WavetableOsc1D
{
public:
	AnalogOscillator();
	~AnalogOscillator();

	inline void setDrift(float p_drift){
		m_drift = p_drift;
	}

	inline void setPWMDuty(float p_duty){
		m_duty = p_duty;
		m_duty = m_duty > 0.98f ? 0.98f : m_duty;
		m_duty = m_duty < 0.02f ? 0.02f : m_duty;
	}

	float doOscillate() override;
	void update() override;
	void loadWavetables() override;
	void setPWMModPointer(float* p_pointer){
		pwm_mod = p_pointer;
	}

	void setSmoothValues(){
		m_duty_smooth = m_duty;
	}

	void setSampleRate(float p_sr) override {
		WavetableOsc1D::setSampleRate(p_sr);
		m_drift_generator.setSampleRate(p_sr);
	}

protected:

	float* pwm_mod;

	float doSquare();
	float generateDrift();
	
	DriftGenerator m_drift_generator;
	
	float m_smoothed_drift_noise = 0.f;
	float m_drift = 0.f;
	float m_duty = 0.5f;
	float m_duty_smooth = 0.5f;
};

