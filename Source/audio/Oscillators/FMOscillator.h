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
	//virtual void startOscillator() override;
	//virtual void stopOscillator() override;
	
	inline void setFMAmount(float p_fm_amount){
		m_FM_amount = p_fm_amount;
	}
	
	inline void setRatio(int p_carrier, int p_modulator){
		m_carrier_ratio = p_carrier;
		m_modulator_ratio = p_modulator;
	}

	inline void setCarrierRatio(int p_carrier){
		m_carrier_ratio = p_carrier;
	}

	inline void setModulatorRatio(int p_modulator){
		m_modulator_ratio = p_modulator;
	}

	inline void setFMExponential(bool p_exp){
		m_FM_is_exponential = p_exp; //else linear
		//reset oscs to get rid of terrible noise due to out of sync:
		reset();
	}

	inline void selectWavetables(int p_wt_carrier, int p_wt_modulator){
		m_carrier_osc.selectWavetable(p_wt_carrier);
		m_modulator_osc.selectWavetable(p_wt_modulator);
	}

	inline void selectCarrierWavetable(int p_wt_carrier){
		m_carrier_osc.selectWavetable(p_wt_carrier);
	}

	inline void selectModulatorWavetable(int p_wt_modulator){
		m_modulator_osc.selectWavetable(p_wt_modulator);
	}

	void setFMModPointer(float* p_pointer){
		m_fm_mod = p_pointer;
	}

	void setCarrierRatioModPointer(float* p_pointer){
		m_carrier_ratio_mod = p_pointer;
	}

	void setModulatorRatioModPointer(float* p_pointer){
		m_modulator_ratio_mod = p_pointer;
	}

protected:

	float* m_fm_mod;
	float* m_carrier_ratio_mod;
	float* m_modulator_ratio_mod;

	bool m_FM_is_exponential = false; // else linear

	float m_FM_amount = 0.f;
	int m_carrier_ratio = 1.f;
	int m_modulator_ratio = 1.f;

	//keep this to have a mod pointer for suboscs
	float m_zero_modulation_dummy = 0.f;

	WavetableOsc1D m_carrier_osc;
	WavetableOsc1D m_modulator_osc;
};

