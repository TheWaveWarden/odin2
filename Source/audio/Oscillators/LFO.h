#pragma once

#include "WavetableOsc1D.h"

class LFO : public WavetableOsc1D {

public:
  LFO() {
    //always use lowest table, since this is LFO and we really dont care about aliasing
    m_sub_table_index = 0;

    //get first random SH value
    m_SH_value = getBipolarRandom();

    m_osc_freq_base = 1.f;
  }
  ~LFO() {}

	void loadWavetables();
  float doOscillate();  

  inline void update() override {

    // --- do the  complete frequency mod
    if (*m_pitch_mod_exp) {
      m_osc_freq_modded =
          m_osc_freq_base * pitchShiftMultiplier((*m_pitch_mod_exp) * 48);
    } else {
      m_osc_freq_modded = m_osc_freq_base;
    }

    m_osc_freq_modded = m_osc_freq_modded > OSC_FO_MAX ? OSC_FO_MAX : m_osc_freq_modded;

    m_increment = m_osc_freq_modded / m_samplerate;
    m_wavetable_inc = WAVETABLE_LENGTH * m_increment;

    // set wavetable pointer
    m_current_table =
        m_wavetable_pointers[m_wavetable_index][m_sub_table_index];


  }

  void setSHActive(bool p_active){
    m_SH_active = p_active;
  }

  float getBipolarRandom(){
    float white_noise = (float)rand();
	  return 2 * (white_noise / RAND_MAX) - 1;
  }

  float doSampleHold();

protected:

  float m_SH_active = false;
  float m_SH_value;

  inline int getTableIndex() { return 0; }

};