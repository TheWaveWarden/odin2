/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 TheWaveWarden
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

#pragma once

#include "BiquadFilter.h"


class BiquadEQ : public BiquadFilter{
    public:

    void setQ(float p_Q);
    void setFreq(float p_freq);
    void setGain(float p_gain);
    void setEQParams(float p_Q, float p_freq, float p_gain);
    void setSampleRate(float p_samplerate) override;

    protected:
    void recalculateCoefficients();

    float m_Q = 1.f;
    float m_freq = 1.f;
    float m_gain = 0.f;
};