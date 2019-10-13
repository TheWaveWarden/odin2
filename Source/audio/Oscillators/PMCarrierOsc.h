/*
  ==============================================================================

    PMCarrierOsc.h
    Created: 19 Jun 2019 6:50:26pm
    Author:  frot

  ==============================================================================
*/

#pragma once

#include "WavetableOsc1D.h"

class PMCarrierOsc : public WavetableOsc1D {
  public:

  void setPhaseMod(float p_phase){
    m_phase_velocity = p_phase - m_last_phasemod;

    m_last_phasemod = m_phasemod;
    m_phasemod = p_phase;
  }

  int getTableIndex() override {
    //Will Pirkle book 653
    //instantaneous frequency:

    //w_i = d(OscArg) / dt = w + d(phaseDiff) / dt = w + phaseVelocity * const

    // const = SR (?) 
    // derived from thought where freq = 0Hz and PM signal is f(t) = t
    // then WT gets run through once per sample and hence freq = SR,
    // the phaseVelocity for f(t) = t is 1 Sample and therefore 1 * const = SR,
    // hence const = SR

    double seed_freq = 27.5; //A0
    float abs_freq = fabs(m_osc_freq_modded + m_phase_velocity * m_samplerate);
    for(int table = 0; table < SUBTABLES_PER_WAVETABLE; table++){
        if(abs_freq < seed_freq){
            return table;
        }
        seed_freq *= 1.1892071150; //minor third up
    }

    return SUBTABLES_PER_WAVETABLE -1;//never gets here
  }

  void update() override {
    //baseclass call
    Oscillator::update();

    m_wavetable_inc = WAVETABLE_LENGTH * m_increment;

    m_sub_table_index = getTableIndex();

    //set wavetable pointer
    m_current_table = m_wavetable_pointers[m_wavetable_index][m_sub_table_index];
  }

  float doOscillate() override {

     //prepare both sides and interpol value
    int read_index_trunc = (int) (m_read_index + m_phasemod * WAVETABLE_LENGTH);
    float fractional = m_read_index + m_phasemod * WAVETABLE_LENGTH - (float)read_index_trunc;
    int read_index_next = read_index_trunc + 1;

    //clamp to zone
    while(read_index_trunc >= WAVETABLE_LENGTH){ read_index_trunc -= WAVETABLE_LENGTH;}
    while(read_index_next >= WAVETABLE_LENGTH){ read_index_next -= WAVETABLE_LENGTH;}
    while(read_index_trunc < WAVETABLE_LENGTH){ read_index_trunc += WAVETABLE_LENGTH;}
    while(read_index_next < WAVETABLE_LENGTH){ read_index_next += WAVETABLE_LENGTH;}

    // do linear interpolation 
    float output = linearInterpolation(m_current_table[read_index_trunc], m_current_table[read_index_next], fractional);

    m_read_index += m_wavetable_inc;
    checkWrapIndex(m_read_index);
    return output;
  }


  protected:
  float m_phase_velocity;
  float m_phasemod = 0;
  float m_last_phasemod = 0;
};


