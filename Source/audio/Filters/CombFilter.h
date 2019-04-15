#pragma once

#include "../OdinConstants.h"
#include "Filter.h"
#include <cstring>

#define COMB_BUFFER_LENGTH (MAX_EXPECTED_SAMPLE_RATE / FILTER_FC_MIN)

//#include "../JuceLibraryCode/JuceHeader.h"//todo remove

class CombFilter
{
  public:
	CombFilter();
	~CombFilter();

	float doFilter(float p_input);
	int m_MIDI_note = 0;
	float m_kbd_mod_amount = 0;
	float m_vel_mod_amount = 0;
	float m_env_mod_amount = 0;
	float m_env_value = 0;


	inline void setPositive(bool p_positive){
		m_positive_comb = p_positive ? 1 : -1;
	}

	inline void setCombFreq(float p_freq)
	{
		//TODO check for too long time... here?
		m_delay_time_control = 1.f / p_freq; 
	}

	inline void setResonance(float p_feedback)
	{
		m_feedback = p_feedback;
	}

	inline void setSamplerate(float p_samplerate)
	{
		m_samplerate = p_samplerate;
	}

	inline void incWriteIndex()
	{
		if (++m_write_index >= COMB_BUFFER_LENGTH)
		{
			m_write_index = 0;
		}
	}

	inline float linearInterpolation(float p_low, float p_high, float p_distance)
	{
		return (1.f - p_distance) * p_low + p_distance * p_high;
	}

	inline virtual void reset()
	{
		m_write_index = 0;
		memset(circular_buffer, 0, COMB_BUFFER_LENGTH * sizeof(float));
	}

	//buffer length is 4 seks at 192kHz or ~17s at 44.1Khz...
	float circular_buffer[COMB_BUFFER_LENGTH] = {0};

	int m_write_index = 0;
	int m_positive_comb = 1;

	//need to be init by synth
	double m_delay_time_control = 1./440.f;
	double m_delay_time_smooth = 1./440.f; //todo what value here?
	float m_feedback = 0;
	float m_samplerate = 44100;

};
