/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 - 2021 TheWaveWarden
**
** Odin 2 is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Odin 2 is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/

#include <cmath>
#include "BiquadEQ.h"


void BiquadEQ::setQ(float p_Q){
    m_Q = p_Q;
    recalculateCoefficients();
}

void BiquadEQ::setFreq(float p_freq){
    m_freq = p_freq;
    recalculateCoefficients();
}

void BiquadEQ::setGain(float p_gain){
    m_gain = p_gain;
    recalculateCoefficients();
}

void BiquadEQ::setEQParams(float p_Q, float p_freq, float p_gain){
    m_Q = p_Q;
    m_freq = p_freq;
    m_gain = p_gain;
    recalculateCoefficients();
}

void BiquadEQ::recalculateCoefficients() {
    //https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html

    float alpha = sin(m_freq * 2. *M_PI * m_one_over_samplerate) / (2.f * m_Q);
    float A = pow(10, m_gain / 40);
    float a_0 = 1. + alpha / A; // we need to divide by this, since form is unnormalized

    m_b0 = (1. + alpha * A) / a_0;
    m_b1 = -2. * cos(m_freq * 2. *M_PI * m_one_over_samplerate) / a_0; 
    m_b2 = (1 - alpha * A) / a_0;
    m_a1 = m_b1;
    m_a2 = (1 - alpha / A) / a_0;
}

void BiquadEQ::setSampleRate(float p_samplerate) {
    BiquadFilter::setSampleRate(p_samplerate);
    recalculateCoefficients();
}
