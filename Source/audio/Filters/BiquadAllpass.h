#pragma once

#include "BiquadFilter.h"
#include "../OdinConstants.h"

class BiquadAllpass : public BiquadFilter
{
public:
	BiquadAllpass();
	~BiquadAllpass();

	inline void setSamplerate(float p_samplerate){
		m_samplerate = p_samplerate;
	}

	inline void setRadius(float p_radius){
		m_radius = p_radius;
	}

	void setFrequency(float p_frequency);


protected:

	float m_samplerate;
	float m_radius;
};

