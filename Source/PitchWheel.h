/*
  ==============================================================================

    PitchWheel.h
    Created: 3 Mar 2019 7:06:49pm
    Author:  frot

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalIncludes.h"
#include "Knob.h"
//==============================================================================
/*
 */
class PitchWheel : public Knob {
public:
  PitchWheel() {
    setRange(-1, 1);
    setNumDecimalPlacesToDisplay(3);
  }

  ~PitchWheel() {}

  void mouseUp(const MouseEvent &e) { 
    Slider::mouseUp(e);
    setValue(0, sendNotification); 
    }

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PitchWheel)
};
