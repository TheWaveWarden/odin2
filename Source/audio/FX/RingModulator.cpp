
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
  return out;
}

void RingModulator::reset() {

}

int RingModulator::getTableIndex() {
  //sine is always the same
  return 0;
}
