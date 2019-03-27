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
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setRange(-1, 1);
    setNumDecimalPlacesToDisplay(3);
  }

  ~PitchWheel() {}

  void mouseUp(const MouseEvent &e) { setValue(0, sendNotification); }

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PitchWheel)
};
