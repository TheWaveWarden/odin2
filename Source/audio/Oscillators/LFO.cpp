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
  setWavetablePointer(0,
                      WavetableContainer::getInstance().getWavedrawPointer(2));
}

float LFO::doOscillate() {
  if (!m_SH_active) {
    return doWavetable();
  } else {
    return doSampleHold();
  }
}

float LFO::doSampleHold() {

  m_read_index += m_wavetable_inc;

  while (m_read_index < 0) {
    m_read_index += WAVETABLE_LENGTH;
    m_SH_value = getBipolarRandom();
  }
  while (m_read_index >= WAVETABLE_LENGTH) {
    m_read_index -= WAVETABLE_LENGTH;
    m_SH_value = getBipolarRandom();    
  }

  return m_SH_value;
}
