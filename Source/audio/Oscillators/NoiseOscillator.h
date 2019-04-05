#pragma once

#include "../Filters/VAOnePoleFilter.h"

class NoiseOscillator
{
public:
	NoiseOscillator();
	~NoiseOscillator();

	float doNoise();   

	void setFilterFreqs(float p_lowpass_freq, float p_highpass_freq);

	//inline void startOscillator(){
	//	m_note_on = true;
	//}
	
	//inline void stopOscillator(){
	//	m_note_on = false;
	//}

protected:

	bool m_note_on = false;

	VAOnePoleFilter m_lowpass;
	VAOnePoleFilter m_highpass;
};

