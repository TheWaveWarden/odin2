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

	virtual float doOscillate() override;
	virtual void update() override;
	virtual void loadWavetables() override;
	void setPWMModPointer(float* p_pointer){
		pwm_mod = p_pointer;
	}

protected:

	float* pwm_mod;

	float doSquare();
	float generateDrift();
	
	DriftGenerator m_drift_generator;
	
	float m_smoothed_drift_noise = 0.f;
	float m_drift = 0.f;
	float m_duty = 0.5f;
};

