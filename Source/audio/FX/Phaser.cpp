#include "Phaser.h"
#include <cmath>
#include "../Filters/BiquadAllpass.h"



Phaser::Phaser()
{
}


Phaser::~Phaser()
{
}

void Phaser::setRadius(float p_radius){
    m_AP1.setRadius(p_radius);
    m_AP2.setRadius(p_radius);
    m_AP3.setRadius(p_radius);
    m_AP4.setRadius(p_radius);
    m_AP5.setRadius(p_radius);
    m_AP6.setRadius(p_radius);
}

void Phaser::setFrequency(float p_frequency){
    m_AP1.setFrequency(p_frequency * 0.8f);
    m_AP2.setFrequency(p_frequency);
    m_AP3.setFrequency(p_frequency * 1.1f);
    m_AP4.setFrequency(p_frequency * 1.2f);
    m_AP5.setFrequency(p_frequency * 0.97f);
    m_AP6.setFrequency(p_frequency * 0.83f);
}


void Phaser::setAmount(float p_amount){
    m_drywet = p_amount * 0.5f;
}

float Phaser::doPhaser(float p_input){
    incrementLFO();
    //setFrequency(m_base_freq * pow(2.f, doLFO() * m_amount));
    float LFO = doLFO();

    float amount_modded = m_amount + *m_amount_mod;
    amount_modded = amount_modded < 0 ? 0 : amount_modded;
    amount_modded = amount_modded > 1.5f ? 1.5f : amount_modded;

    setRadius(m_radius_base + 0.15f * LFO * amount_modded);
    setFrequency((LFO + 1 ) * amount_modded * PHASER_MAX_LFO_AMPLITUDE);

    double phase_shifted = m_AP1.doFilter(m_AP2.doFilter(m_AP3.doFilter(p_input)));
    phase_shifted = m_AP4.doFilter(m_AP5.doFilter(m_AP6.doFilter(phase_shifted)));

    float drywet_modded = m_drywet + *m_drywet_mod * 0.5f;
    drywet_modded = drywet_modded > 0.5f ? 0.5f : drywet_modded;
    drywet_modded = drywet_modded < 0 ? 0 : drywet_modded;

    return (float)((1.f - drywet_modded) * p_input + drywet_modded * phase_shifted);
}
