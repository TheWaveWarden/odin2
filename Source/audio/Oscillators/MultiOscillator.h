#pragma once
#include "WavetableOsc2D.h"

#define OSCS_PER_MULTIOSC 4

class MultiOscillator :
	public WavetableOsc2D
{
public:
	MultiOscillator();
	~MultiOscillator();

	float doOscillate() override;
	void update() override;
	void reset() override;

	void randomizePhase(){
		for(int osc = 0; osc < OSCS_PER_MULTIOSC; ++osc){
	  		m_read_index_multi[osc] = (float)rand() / (float)RAND_MAX * WAVETABLE_LENGTH;
		}
	}

	inline void setPosition(float p_position){
		m_position_2D = p_position;
	}

	inline void setDetune(float p_detune){
		m_detune = p_detune * p_detune;
	}

	inline void setWavetableMultiSpread(float p_wt_spread){
		m_wavetable_multi_spread = p_wt_spread;
	}

	void setDetuneModPointer(float* p_pointer){
		m_detune_mod = p_pointer;
	}

	void setSpreadModPointer(float* p_pointer){
		m_spread_mod = p_pointer;
	}
	
protected:
	
	void initiateSync() override {
		for (int subosc = 0; subosc < OSCS_PER_MULTIOSC; ++subosc){
			m_read_index_multi[subosc] = 0;
		}
		m_read_index = 0;
	}


	float* m_detune_mod;
	float* m_spread_mod;

	float cheapPitchShiftMultiplier(float p_semitones);

	int getTableIndex(float p_freq);
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
	float m_detune = 0.04f;//0.2²
	float m_wavetable_multi_spread = 0.f;

	float m_position_2D_multi[OSCS_PER_MULTIOSC];
	double m_read_index_multi[OSCS_PER_MULTIOSC] = {0.f};
	float m_oscillator_freq_multi[OSCS_PER_MULTIOSC];
	float m_wavetable_inc_multi[OSCS_PER_MULTIOSC];
	int m_sub_table_index_multi[OSCS_PER_MULTIOSC];
	const float** m_current_table_2D_multi[OSCS_PER_MULTIOSC];

	//#include "PitchShiftTable.h" //contains 1000 points of pitch shift table for -5 to +5
};

