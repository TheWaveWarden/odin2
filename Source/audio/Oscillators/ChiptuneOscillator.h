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
#include "WavetableOsc1D.h"
#include "ChiptuneArpeggiator.h"
#include <cstdlib>


#define NR_CHIPTUNE_WAVETABES 10

class ChiptuneOscillator : public WavetableOsc1D{
public:
	ChiptuneOscillator();
	~ChiptuneOscillator();

    inline void setBaseFrequency(float p_freq) override {
        WavetableOsc1D::setBaseFrequency(p_freq);
        m_chiptune_arp.reset();
    }

    float doOscillate() override;
    void update() override;
    void setSampleRate(float p_samplerate) override;
    void reset() override;

    float generateChipNoise();

    inline void setArpSpeed(float p_speed){
        m_chiptune_arp.setFrequency(p_speed);   
    }

    inline void setArpSemitone(int p_step, int p_value){
        m_chiptune_arp.setStepSemitone(p_step, p_value);
    }

    inline void setArpEnabled(bool p_enabled){
	    m_chiptune_arp.setArpeggiatorOn(p_enabled);
    }

    inline void setArpStepThreeOn(bool p_on){
        m_chiptune_arp.setStepThreeActive(p_on);
    }

    inline void setNoiseEnabled(bool p_noise){
        if(p_noise){
            setOperationModeNoise();
        } else {
            setOPerationModeWave();
        }
    }

    inline void setOperationModeNoise(){
        if(!m_generate_noise){
            m_read_index = 0.0;
            m_generate_noise = true;
        }
    }

    inline void setOPerationModeWave(){
        if(m_generate_noise){
            m_generate_noise = false;
            m_read_index = 0.0;
        }
    }

    inline bool getOperationModeNoise(){
        return m_generate_noise;
    }

    void setArpSpeedModPointer(float* p_pointer){
        m_chiptune_arp.setFreqModPointer(p_pointer);
    }


    ChiptuneArpeggiator m_chiptune_arp;
    bool m_generate_noise = false; //set when noise should be generated
    float m_last_noise_value = 0.0;

    //buffer elements for downsampling filter in noise generation
    float xv[10];
    float yv[10];
};

