#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class OdinMidiLearnBase {
public:
  void setParameterID(const String& p_ID){
    m_parameter_ID = p_ID;
    DBG("Set parameter ID in Knob: " + p_ID);
  }

  virtual void setMidiControlActive() = 0;
protected:
  String m_parameter_ID;
};
