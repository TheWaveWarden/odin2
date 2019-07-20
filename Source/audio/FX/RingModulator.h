#pragma once

#include "../Oscillators/WavetableOsc1D.h"



class RingModulator : public WavetableOsc1D{
public:
  RingModulator();
  float doRingModulator(float p_input);
  void reset() override;
  void update() override;

  virtual void setVolModPointer(float *p_pointer) { m_vol_mod = p_pointer; }
  virtual void setVelModPointer(float *p_pointer) { m_vel_mod_mod = p_pointer; }
  virtual void setKbdModPointer(float *p_pointer) { m_kbd_mod_mod = p_pointer; }
  virtual void setEnvModPointer(float *p_pointer) { m_env_mod_mod = p_pointer; }
  virtual void setRingModAmountModPointer(float *p_pointer) { m_ringmod_amount_mod = p_pointer; }
  
  void setAmount(float p_amount){
    m_amount = p_amount;
  }

  float m_kbd_mod_amount = 0;
  int m_MIDI_note = 0;
  float m_env_value = 0;
  float m_env_mod_amount = 0;
  int m_MIDI_velocity = 0;
  float m_vel_mod_amount = 0;

protected:
  int getTableIndex() override;

  float m_modulation_dummy = 0;

  //float *m_vol_mod;
  float *m_vel_mod_mod;
  float *m_kbd_mod_mod;
  float *m_env_mod_mod;
  float *m_ringmod_amount_mod;
  
  float m_amount = 1;


};
