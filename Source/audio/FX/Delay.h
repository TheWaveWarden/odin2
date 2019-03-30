#pragma once

#include "../OdinConstants.h"
#include "../Filters/VAOnePoleFilter.h"

//#include <memory>
#include <cstring>

#define CIRCULAR_BUFFER_LENGTH (MAX_EXPECTED_SAMPLE_RATE * MAX_DELAY_TIME)
#define MOVING_AVERAGE_SAMPLE_LENGTH 441 //1/100 s at standard smaple rate
#define DUCKING_SCALE_FACTOR 5 // hardly ever will the average reach 1, so we scale it up


class Delay
{
public:
	Delay();
	~Delay();

	float doDelay(float p_input);

	inline void setDelayTime(float p_delay_time){
		//TODO check for too long time... here?
		m_delay_time_control = p_delay_time;
	}

	inline void setFeedback(float p_feedback){
		m_feedback = p_feedback;
	}

	inline void setSampleRate(float p_samplerate){
		m_samplerate = p_samplerate;
	} 

	inline void incWriteIndex(){
		if(++m_write_index >= CIRCULAR_BUFFER_LENGTH){
			m_write_index = 0;
		}
	}

	inline void setHPFreq(float p_freq){
		m_highpass_freq = p_freq;
		m_highpass.m_freq_base = p_freq;
	}

	inline float linearInterpolation(float p_low, float p_high, float p_distance){
		return (1.f - p_distance) * p_low + p_distance * p_high;
	}

	inline void reset(){
		m_write_index = 0;
		memset(circular_buffer, 0, CIRCULAR_BUFFER_LENGTH * sizeof(float));

		m_highpass.reset();
	}

	inline void setDry(float p_dry){
		m_dry = p_dry;
	}

	inline void setWet(float p_wet){
		m_wet = p_wet;
	}

	inline void setDucking(float p_ducking){
		m_ducking_amount = p_ducking;
	}

	inline float doAverage(float p_input){
		//abs input for pos only
		p_input = fabs(p_input);

		//do moving average
		m_total_sum += p_input - m_last_mean;
        m_last_mean = m_total_sum / MOVING_AVERAGE_SAMPLE_LENGTH;
        return m_last_mean * DUCKING_SCALE_FACTOR > 1 ? 1 : m_last_mean * DUCKING_SCALE_FACTOR;
	}

	//buffer length is 4 seks at 192kHz or ~17s at 44.1Khz...
	float circular_buffer[CIRCULAR_BUFFER_LENGTH] = {0};

	VAOnePoleFilter m_highpass;

	int m_write_index = 0;
	
	//moving average
	float m_total_sum = 0.f;
	float m_last_mean = 0.f;

	//need to be init by synth
	float m_delay_time_control;
	float m_delay_time_smooth = 0.5f;//todo what value here?
	float m_feedback;
	float m_samplerate;
	float m_dry;
	float m_wet;
	float m_highpass_freq;
	float m_ducking_amount;
};

