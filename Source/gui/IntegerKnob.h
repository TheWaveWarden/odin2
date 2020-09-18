#pragma once

#include "../GlobalIncludes.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "Knob.h"

class IntegerKnob : public Knob {
public:
  IntegerKnob();
  ~IntegerKnob();

  virtual double snapValue(double attemptedValue, DragMode dragMode){
    return round(attemptedValue);
  }

      private : JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IntegerKnob)
};
