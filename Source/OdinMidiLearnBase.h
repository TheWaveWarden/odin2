#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class OdinMidiLearnBase {
public:

  void setParameterID(const String& p_ID){
    m_parameter_ID = p_ID;
    DBG("Set parameter ID in Knob: " + p_ID);
  }

  virtual void setMidiControlActive() = 0;
  virtual void stopMidiLearn() = 0;

  void disableMidiLearn(){
    m_midi_learn_possible = false;
  }
protected:

  bool m_midi_learn_possible = true;
  bool m_midi_learn = false;
  bool m_midi_control = false;
  String m_parameter_ID;
};
