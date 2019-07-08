
#include "RingModulator.h"


RingModulator::RingModulator(){
  m_glide_mod = &m_modulation_dummy;
  m_pitch_mod_exp = &m_modulation_dummy;
  m_pitch_mod_lin = &m_modulation_dummy;
  m_vol_mod = &m_modulation_dummy;
  m_pitchbend = &m_modulation_dummy;

  m_nr_of_wavetables = 1;
}


float RingModulator::doRingModulator(float p_input){
  float out = p_input * doOscillate();
  DBG(out);
  DBG(p_input);
  return out;
  
}

void RingModulator::reset() {

}