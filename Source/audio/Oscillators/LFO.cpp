/*
  ==============================================================================

    LFO.cpp
    Created: 25 Apr 2019 11:24:01pm
    Author:  frot

  ==============================================================================
*/

#include "LFO.h"

void LFO::loadWavetables() {
  for (int wavetable = 0; wavetable < NUMBER_OF_WAVETABLES; ++wavetable) {
    setWavetablePointer(
        wavetable,
        WavetableContainer::getInstance().getWavetablePointers(wavetable));
  }
}

float LFO::doOscillate() {
  return doWavetable();
}
