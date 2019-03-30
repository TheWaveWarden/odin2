#pragma once

#include "Oscillator.h"
#include "WavetableOsc1D.h"

#define EXP_FM_SEMITONES 24

class FMOscillator :
	public Oscillator
{
public:
	FMOscillator();
	~FMOscillator();

	virtual void update() override;
	virtual void reset() override;
	virtual float doOscillate() override;
	virtual void loadWavetables();
	virtual void startOscillator() override;
	virtual void stopOscillator() override;
	
	inline void setFMAmount(float p_fm_amount){
		m_FM_amount = p_fm_amount;
	}
	
	inline void setRatio(int p_carrier, int p_modulator){
		m_carrier_ratio = p_carrier;
		m_modulator_ratio = p_modulator;
	}

	inline void setFMExponential(bool p_exp){
		m_FM_is_exponential = p_exp; //else linear
	}

	inline void selectWavetables(int p_wt_carrier, int p_wt_modulator){
		m_carrier_osc.selectWavetable(p_wt_carrier);
		m_modulator_osc.selectWavetable(p_wt_modulator);
	}

protected:

	bool m_FM_is_exponential = false; // else linear

	float m_FM_amount = 0.f;
	int m_carrier_ratio = 1.f;
	int m_modulator_ratio = 1.f;

	WavetableOsc1D m_carrier_osc;
	WavetableOsc1D m_modulator_osc;
};

