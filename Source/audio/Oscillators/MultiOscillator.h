#pragma once
#include "WavetableOsc2D.h"

#define OSCS_PER_MULTIOSC 4

class MultiOscillator :
	public WavetableOsc2D
{
public:
	MultiOscillator();
	~MultiOscillator();

	virtual float doOscillate() override;
	virtual void update() override;
	virtual void reset() override;

	inline void setPosition(float p_position){
		m_position_2D = p_position;
	}

	inline void setDetune(float p_detune){
		m_detune = p_detune;
	}

	inline void setWavetableMultiSpread(float p_wt_spread){
		m_wavetable_multi_spread = p_wt_spread;
	}

	inline void setResetActive(bool p_reset){
		m_reset_active = p_reset;
	}

	virtual void loadWavetables() override; 

	//inline virtual void startOscillator() override{
	//	//m_note_on = true;
	//	if(m_reset_active){
	//		for(int osc = 0; osc < OSCS_PER_MULTIOSC; ++osc){
	//			m_read_index_multi[osc] = 0.f;
	//		}
	//	}
	//}

protected:

	double cheapPitchShiftMultiplier(double p_semitones);

	virtual int getTableIndex() override;
	float doWavetableMulti();

	inline void getTableIndicesAndInterpolation(int& p_left_table, int& p_right_table, float& p_interpolation_value, float p_position_2D){
		if(p_position_2D < 0.333333333333f){
			p_left_table = 0;
			p_right_table = 1;
			p_interpolation_value = p_position_2D * 3;
			return;
		} else if(p_position_2D < 0.666666666f){
			p_left_table = 1;
			p_right_table = 2;
			p_interpolation_value = (p_position_2D - 0.333333333f) * 3;
			return;
		} 
		p_left_table = 2;
		p_right_table = 3;
		p_interpolation_value = (p_position_2D - 0.6666666f) * 3;
		return;
	}

	float m_position_2D = 0.f;
	float m_detune = 0.f;
	bool m_reset_active = false;
	float m_wavetable_multi_spread = 0.f;

	float m_position_2D_multi[OSCS_PER_MULTIOSC];
	double m_read_index_multi[OSCS_PER_MULTIOSC] = {0.f};
	float m_oscillator_freq_multi[OSCS_PER_MULTIOSC];
	float m_wavetable_inc_multi[OSCS_PER_MULTIOSC];
	int m_sub_table_index_multi[OSCS_PER_MULTIOSC];
	float** m_current_table_2D_multi[OSCS_PER_MULTIOSC];

	#include "PitchShiftTable.h" //contains 1000 points of pitch shift table for -5 to +5
};

