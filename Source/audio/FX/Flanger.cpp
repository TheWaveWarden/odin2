#include "Flanger.h"



Flanger::Flanger()
{
}


Flanger::~Flanger()
{
}


float Flanger::doFlanger(float p_input){
    
    incrementLFO();
    float LFO = doLFO();
    m_delay_time_control = m_base_time + LFO * m_LFO_amount * FLANGER_LFO_MAX_RANGE;

    return m_dry_wet * doFilter(p_input) + (1.f - m_dry_wet) * p_input;
}
