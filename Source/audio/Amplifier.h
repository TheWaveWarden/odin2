#pragma once

#include "../JuceLibraryCode/JuceHeader.h" //for db to gain

#define MIDI_VEL_MAX 127.f

class Amplifier {

public:
  inline void doAmplifier(float p_in, float &po_left_out, float &po_right_out) {

    // appply gain
    float gain_mod_factor =
        (*m_gain_mod) > 0 ? 1.f + 4 * (*m_gain_mod) : (1.f + *m_gain_mod);
    p_in *= m_gain * gain_mod_factor;

    // apply velocity
    float vel_modded = m_vel_amount + *(m_vel_mod);
    vel_modded = vel_modded < 0 ? 0 : vel_modded;
    vel_modded = vel_modded > 1 ? 1 : vel_modded;
    p_in *= 1.f - vel_modded * 1.f + vel_modded * m_MIDI_vel;

    // do panning
    float pan_modded = m_pan + *m_pan_mod;
    pan_modded = pan_modded < -1 ? -1 : pan_modded;
    pan_modded = pan_modded > 1 ? 1 : pan_modded;
    if (pan_modded >= 0.f) {
      po_right_out = p_in;
      po_left_out = (1.f - pan_modded) * p_in;
    } else {
      po_right_out = (pan_modded + 1.f) * p_in;
      po_left_out = p_in;
    }
  }

  inline void setGainDecibels(float p_dB) {
    // convert dB to multiplicator
    m_gain = Decibels::decibelsToGain(p_dB);
  }

  inline void setPan(float p_pan) { m_pan = p_pan; }

  // this is for the knob
  inline void setVelocityAmount(float p_vel) { m_vel_amount = p_vel; }

  // this is for the keypress value
  inline void setMIDIVelocity(int p_MIDI_vel) {
    m_MIDI_vel = (float)p_MIDI_vel / MIDI_VEL_MAX;
  }

  void setGainModPointer(float *p_pointer) { m_gain_mod = p_pointer; }
  void setPanModPointer(float *p_pointer) { m_pan_mod = p_pointer; }
  void setVelModPointer(float *p_pointer) { m_vel_mod = p_pointer; }

protected:
  float *m_gain_mod;
  float *m_pan_mod;
  float *m_vel_mod;

  float m_MIDI_vel = 0.f;
  float m_vel_amount = 0.f;
  float m_gain = 1.f; // this is a multiplicator, not a dB value
  float m_pan = 0.f;
};