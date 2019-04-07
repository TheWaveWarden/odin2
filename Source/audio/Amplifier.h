#pragma once

#include "../JuceLibraryCode/JuceHeader.h"//for db to gain

#define MIDI_VEL_MAX 127.f



class Amplifier {

public:

  inline void doAmplifier(float p_in, float& po_left_out, float& po_right_out){

    //appply gain
    p_in *= m_gain;

    //apply velocity
    p_in *= 1.f - m_vel_amount * 1.f + m_vel_amount * m_MIDI_vel;

    //do panning
    if(m_pan >= 0.f){
      po_right_out = p_in;
      po_left_out = (1.f - m_pan) * p_in;
    } else {
      po_right_out = (m_pan + 1.f) * p_in;
      po_left_out = p_in;
    }
  }

  inline void setGainDecibels(float p_dB){
    //convert dB to multiplicator
    m_gain = Decibels::decibelsToGain(p_dB);
  }

  inline void setPan(float p_pan){
    m_pan = p_pan;
  }

  //this is for the knob
  inline void setVelocityAmount(float p_vel){
    m_vel_amount = p_vel;
  }

  //this is for the keypress value
  inline void setMIDIVelocity(int p_MIDI_vel){
    m_MIDI_vel = (float)p_MIDI_vel / MIDI_VEL_MAX;
  }

protected:

  float m_MIDI_vel = 0.f;
  float m_vel_amount = 0.f;
  float m_gain = 1.f;//this is a multiplicator, not a dB value
  float m_pan = 0.f;

};