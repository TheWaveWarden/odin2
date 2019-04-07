#pragma once
#include "WavetableOsc1D.h"
class VectorOscillator :
	public WavetableOsc1D
{
public:
	VectorOscillator();
	~VectorOscillator();

	inline void setXYPad(float p_x, float p_y){
		m_XY_pad_x = p_x;
		m_XY_pad_y = p_y;
	}

	inline void setX(float p_x){
		m_XY_pad_x = p_x;
	}

	inline void setY(float p_y){
		m_XY_pad_y = p_y;
	}
	
	void selectWavetable(int p_wavetable_index, int p_vector_point);

	virtual float doOscillate() override;
	virtual void update() override;
	virtual void loadWavetables() override;

protected:

	float doVectortable();

	float m_XY_pad_x = 0.f;
	float m_XY_pad_y = 0.f;

	float* m_current_table_vec_0;
	float* m_current_table_vec_1;
	float* m_current_table_vec_2;
	float* m_current_table_vec_3;

	int m_wavetable_index_0 = 0;
	int m_wavetable_index_1 = 0;
	int m_wavetable_index_2 = 0;
	int m_wavetable_index_3 = 0;
};

