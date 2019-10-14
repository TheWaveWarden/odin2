#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define STEP_LENGTH 1

class ChiptuneArpeggiator
{
public:
    ChiptuneArpeggiator() {}
    ~ChiptuneArpeggiator() {}

    inline void reset()
    {
        m_current_step = 0;
        m_position_in_step = 0.f;
    }

    inline void setStepThreeActive(bool p_active)
    {
        m_step_three_active = p_active;
    }

    inline void setStepSemitone(int p_step, int p_semitone)
    {
        m_step_semitones[p_step] = p_semitone;
    }

    inline void setFrequency(float p_frequency)
    {
        m_frequency = p_frequency;
    }

    inline void setArpeggiatorOn(bool p_arp_on)
    {
        m_arp_active = p_arp_on;
    }

    inline void setSampleRate(float p_samplerate)
    {

        m_samplerate = p_samplerate;
    }

    int doArpeggiator();

    void setFreqModPointer(float *p_pointer)
    {
        m_freq_mod = p_pointer;
    }

protected:
    float *m_freq_mod;
    float m_frequency = 20.f;
    float m_samplerate = 44100;
    float m_inc;
    float m_position_in_step = 0.f;
    int m_current_step = 0;
    int m_step_semitones[3] = {0, 12, 7};
    bool m_step_three_active = false;
    bool m_arp_active = false;
    //float m_frequency = 0.01f; //doesn't necessarily need to be in Hz

    //precomputed pitch shift values for fast access
    //float m_precomputed_pitch_factors[25] = {
    //    0.5f, 0.529732f, 0.561231f, 0.594604f, 0.629961f, 0.66742f, 0.707107f, 0.749154f, 0.793701f, 0.840896f, 0.890899f, 0.943874f, 1.0f, 1.05946f, 1.12246f, 1.18921f, 1.25992f, 1.33484f, 1.41421f, 1.49831f, 1.5874f, 1.68179f, 1.7818f, 1.88775f, 2.0f};
};
