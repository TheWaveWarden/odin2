#include "Delay.h"

Delay::Delay()
{
    m_highpass.setHP();
    m_highpass.reset();
}


Delay::~Delay()
{
}


float Delay::doDelay(float p_input){

    //TODO implement sync mode
    //todo audio bug when increasing delay_time

	m_delay_time_smooth =  (m_delay_time_smooth - m_delay_time_control) * 0.9999 + m_delay_time_control;


    m_highpass.update(); //needs to be done only for modding later

    //calc read index
    float read_index = (float)m_write_index - m_delay_time_smooth * m_samplerate;
    int read_index_trunc = (int)read_index;
    int read_index_next = read_index_trunc + 1;
    float frac = read_index - (float)read_index_trunc;

    //check boundaries
    read_index_trunc = read_index_trunc < 0 ? read_index_trunc + CIRCULAR_BUFFER_LENGTH : read_index_trunc; 
    read_index_next  = read_index_next  < 0 ? read_index_next  + CIRCULAR_BUFFER_LENGTH : read_index_next;

    float output = linearInterpolation(circular_buffer[read_index_trunc], circular_buffer[read_index_next], frac);

    circular_buffer[m_write_index] = p_input + output * m_feedback;
    incWriteIndex();

    //set sample behind readindex to zero to avoid reading that signal when increasing delay time
    circular_buffer[read_index_trunc] = 0.f;

    //apply HP filter
    output = m_highpass.doFilter(output);

    //apply ducking
    output *= (1.f - doAverage(p_input) * m_ducking_amount);
    
    return output * m_wet + p_input * m_dry;
}
