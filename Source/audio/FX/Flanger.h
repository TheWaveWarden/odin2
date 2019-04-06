#pragma once

#include "../Filters/CombFilter.h"

#define FLANGER_LFO_MAX_RANGE 0.0095f // 0.001 to 0.02seconds

class Flanger : public CombFilter
{
public:
	Flanger();
	~Flanger();

	inline void reset() override {
		CombFilter::reset();
		m_index_sine = 0;
	}

	float doFlanger(float p_input);

	inline void setBaseTime(float p_base_time){
		m_base_time = p_base_time;
	}

	inline void setLFOFreq(float p_LFO_freq){
		m_increment_sine = p_LFO_freq / m_samplerate * 2;
	}

	inline void setLFOAmount(float p_LFO_amount){
		m_LFO_amount = p_LFO_amount;
	}

	inline void setDryWet(float p_dry_wet){
		m_dry_wet = p_dry_wet;
	}


protected:
	inline void incrementLFO(){
		m_index_sine += m_increment_sine;
		while(m_index_sine > 1){
			m_index_sine -= 1.f;
			m_LFO_sign *= -1;
		}
	}

	inline float doLFO(){
		//cheap approximation by parabola
		return 4 * (m_index_sine * (1 - m_index_sine)) * m_LFO_sign;
	}
	
	int m_LFO_sign = 1;

	float m_base_time = 0.0105;
	float m_LFO_freq;
	float m_LFO_amount;

	float m_index_sine = 0;
	float m_increment_sine;

	float m_dry_wet;

};

