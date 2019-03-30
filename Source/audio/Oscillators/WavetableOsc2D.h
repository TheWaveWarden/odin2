#pragma once
#include "WavetableOsc1D.h"
class WavetableOsc2D :
	public WavetableOsc1D
{
public:
	WavetableOsc2D();
	~WavetableOsc2D();

	virtual float doOscillate() override;
	virtual void update() override;

	inline void setPosition(float p_position){
		m_position_2D = p_position;
	}

	virtual void loadWavetables() override; 

protected:

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

	void setWavetablePointer(int p_wavetable_index, int p_2D_sub_table, float* p_wavetable_pointers[SUBTABLES_PER_WAVETABLE]);
	float doWavetable2D();
	
	float* m_wavetable_pointers_2D[NUMBER_OF_WAVETABLES_2D][SUBTABLES_PER_WAVETABLE][TABLES_PER_2D_WT];
	float** m_current_table_2D;

};

