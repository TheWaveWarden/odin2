#include "../JuceLibraryCode/JuceHeader.h"

#include <functional>
#pragma once

class OdinTreeListener : public AudioProcessorValueTreeState::Listener {
public:
  std::function<void(const String&, float)> onValueChange;

  void parameterChanged (const String &parameterID, float newValue) override{
    onValueChange(parameterID, newValue);
  }
};