#include "Phaser.h"
#include <cmath>
#include "../Filters/BiquadAllpass.h"



Phaser::Phaser()
{
    setBaseFreq(4000.f);
    setRadiusBase(1.32);
}

Phaser::~Phaser()
{
}

void Phaser::setRadius(float p_radius){
    m_AP1_left.setRadius(p_radius);
    m_AP2_left.setRadius(p_radius);
    m_AP3_left.setRadius(p_radius);
    m_AP4_left.setRadius(p_radius);
    m_AP5_left.setRadius(p_radius);
    m_AP6_left.setRadius(p_radius);
    m_AP7_left.setRadius(p_radius);
    m_AP8_left.setRadius(p_radius);
    m_AP9_left.setRadius(p_radius);
    m_AP10_left.setRadius(p_radius);
    m_AP11_left.setRadius(p_radius);
    m_AP12_left.setRadius(p_radius);

    m_AP1_right.setRadius(p_radius);
    m_AP2_right.setRadius(p_radius);
    m_AP3_right.setRadius(p_radius);
    m_AP4_right.setRadius(p_radius);
    m_AP5_right.setRadius(p_radius);
    m_AP6_right.setRadius(p_radius);
    m_AP7_right.setRadius(p_radius);
    m_AP8_right.setRadius(p_radius);
    m_AP9_right.setRadius(p_radius);
    m_AP10_right.setRadius(p_radius);
    m_AP11_right.setRadius(p_radius);
    m_AP12_right.setRadius(p_radius);
}

void Phaser::setFrequencyLeft(float p_frequency){
    m_AP1_left.setFrequency(p_frequency * (1.f - 0.4f * m_width));
    m_AP2_left.setFrequency(p_frequency);
    m_AP3_left.setFrequency(p_frequency * (1.f + 0.2f * m_width));
    m_AP4_left.setFrequency(p_frequency * (1.f + 0.4f * m_width));
    m_AP5_left.setFrequency(p_frequency * (1.f - 0.13f * m_width));
    m_AP6_left.setFrequency(p_frequency * (1.f - 0.34f * m_width));
    m_AP7_left.setFrequency(p_frequency * (1.f - 0.375f * m_width));
    m_AP8_left.setFrequency(p_frequency * (1.f - 0.23f * m_width));
    m_AP9_left.setFrequency(p_frequency * (1.f + 0.13f * m_width));
    m_AP10_left.setFrequency(p_frequency * (1.f + 0.36f * m_width));
    m_AP11_left.setFrequency(p_frequency * (1.f - 0.09f * m_width));
    m_AP12_left.setFrequency(p_frequency * (1.f - 0.29f * m_width));
}

void Phaser::setFrequencyRight(float p_frequency){
    m_AP1_right.setFrequency(p_frequency * (1.f - 0.4f * m_width));
    m_AP2_right.setFrequency(p_frequency);
    m_AP3_right.setFrequency(p_frequency * (1.f + 0.2f * m_width));
    m_AP4_right.setFrequency(p_frequency * (1.f + 0.4f * m_width));
    m_AP5_right.setFrequency(p_frequency * (1.f - 0.13f * m_width));
    m_AP6_right.setFrequency(p_frequency * (1.f - 0.34f * m_width));
    m_AP7_right.setFrequency(p_frequency * (1.f - 0.375f * m_width));
    m_AP8_right.setFrequency(p_frequency * (1.f - 0.23f * m_width));
    m_AP9_right.setFrequency(p_frequency * (1.f + 0.13f * m_width));
    m_AP10_right.setFrequency(p_frequency * (1.f + 0.36f * m_width));
    m_AP11_right.setFrequency(p_frequency * (1.f - 0.09f * m_width));
    m_AP12_right.setFrequency(p_frequency * (1.f - 0.29f * m_width));
}


void Phaser::setDryWet(float p_amount){
    m_drywet = p_amount * 0.5f;
}

float Phaser::doPhaserLeft(float p_input){

    //apply feedback
    float input_with_feedback = p_input + m_store_output_left * m_feedback;

    incrementLFOLeft();
    float LFO = doLFOLeft();

    float amount_modded = m_amount + *m_amount_mod;
    amount_modded = amount_modded < 0 ? 0 : amount_modded;
    amount_modded = amount_modded > 1.5f ? 1.5f : amount_modded;

    setFrequencyLeft(m_base_freq + (LFO) * amount_modded * PHASER_MAX_LFO_AMPLITUDE);

    double phase_shifted = m_AP1_left.doFilter(m_AP2_left.doFilter(m_AP3_left.doFilter(input_with_feedback)));
    phase_shifted = m_AP4_left.doFilter(m_AP5_left.doFilter(m_AP6_left.doFilter(phase_shifted)));
    phase_shifted = m_AP7_left.doFilter(m_AP8_left.doFilter(m_AP9_left.doFilter(phase_shifted)));
    phase_shifted = m_AP10_left.doFilter(m_AP11_left.doFilter(m_AP12_left.doFilter(phase_shifted)));

    float drywet_modded = m_drywet + *m_drywet_mod * 0.5f;
    drywet_modded = drywet_modded > 0.5f ? 0.5f : drywet_modded;
    drywet_modded = drywet_modded < 0 ? 0 : drywet_modded;

    m_store_output_left = phase_shifted;

    return (float)((1.f - drywet_modded) * p_input + drywet_modded * phase_shifted);
}


float Phaser::doPhaserRight(float p_input){

    //apply feedback
    float input_with_feedback = p_input + m_store_output_right * m_feedback;

    incrementLFORight();
    float LFO = doLFORight();

    float amount_modded = m_amount + *m_amount_mod;
    amount_modded = amount_modded < 0 ? 0 : amount_modded;
    amount_modded = amount_modded > 1.5f ? 1.5f : amount_modded;

    setFrequencyRight(m_base_freq + (LFO) * amount_modded * PHASER_MAX_LFO_AMPLITUDE);

    double phase_shifted = m_AP1_right.doFilter(m_AP2_right.doFilter(m_AP3_right.doFilter(input_with_feedback)));
    phase_shifted = m_AP4_right.doFilter(m_AP5_right.doFilter(m_AP6_right.doFilter(phase_shifted)));
    phase_shifted = m_AP7_right.doFilter(m_AP8_right.doFilter(m_AP9_right.doFilter(phase_shifted)));
    phase_shifted = m_AP10_right.doFilter(m_AP11_right.doFilter(m_AP12_right.doFilter(phase_shifted)));

    float drywet_modded = m_drywet + *m_drywet_mod * 0.5f;
    drywet_modded = drywet_modded > 0.5f ? 0.5f : drywet_modded;
    drywet_modded = drywet_modded < 0 ? 0 : drywet_modded;

    m_store_output_right = phase_shifted;

    return (float)((1.f - drywet_modded) * p_input + drywet_modded * phase_shifted);
}

