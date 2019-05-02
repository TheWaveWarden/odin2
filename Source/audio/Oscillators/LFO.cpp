/*
  ==============================================================================

    LFO.cpp
    Created: 25 Apr 2019 11:24:01pm
    Author:  frot

  ==============================================================================
*/

#include "LFO.h"

void LFO::loadWavetables() {
  //for (int wavetable = 0; wavetable < NUMBER_OF_LFOTABLES; ++wavetable) {
  //  setWavetablePointer(
  //      wavetable,
  //      WavetableContainer::getInstance().getWavetablePointers(wavetable));
  //}
  //setWavetablePointer(0,
  //                    WavetableContainer::getInstance().getWavedrawPointer(2));
  setWavetablePointer(0, WavetableContainer::getInstance().getWavetablePointers("Sine"));
  setWavetablePointer(1, WavetableContainer::getInstance().getWavetablePointers("Saw"));
  setWavetablePointer(2, WavetableContainer::getInstance().getWavetablePointers("Triangle"));

  setWavetablePointer(3, WavetableContainer::getInstance().getLFOPointers("Square50"));
  setWavetablePointer(4, WavetableContainer::getInstance().getLFOPointers("Square25"));
  setWavetablePointer(5, WavetableContainer::getInstance().getLFOPointers("Square12"));

  setWavetablePointer(6, WavetableContainer::getInstance().getLFOPointers("Spike"));

  setWavetablePointer(7, WavetableContainer::getInstance().getLFOPointers("Pyramid4"));
  setWavetablePointer(8, WavetableContainer::getInstance().getLFOPointers("Pyramid6"));
  setWavetablePointer(9, WavetableContainer::getInstance().getLFOPointers("Pyramid8"));
  setWavetablePointer(10, WavetableContainer::getInstance().getLFOPointers("Pyramid12"));

  setWavetablePointer(11, WavetableContainer::getInstance().getLFOPointers("Stair3"));
  setWavetablePointer(12, WavetableContainer::getInstance().getLFOPointers("Stair4"));
  setWavetablePointer(13, WavetableContainer::getInstance().getLFOPointers("Stair6"));
  setWavetablePointer(14, WavetableContainer::getInstance().getLFOPointers("Stair8"));
  setWavetablePointer(15, WavetableContainer::getInstance().getLFOPointers("Stair12"));

  setWavetablePointer(16, WavetableContainer::getInstance().getWavedrawPointer(0));
  setWavetablePointer(17, WavetableContainer::getInstance().getWavedrawPointer(1));
  setWavetablePointer(18, WavetableContainer::getInstance().getWavedrawPointer(2));
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



/*    // generate some files here todo remove
    float sections[4] = {-1.f, 0.f, 1.f, 0.f};
    WavetableContainer::getInstance().createLFOCoefficientsFromConstSections(
        0, sections, 4, "Pyramid4");

    float p6[6] = {-1.f, -0.333333f, 0.3333333f, 1.f, 0.3333333f, -0.33333333f};
    WavetableContainer::getInstance().createLFOCoefficientsFromConstSections(
        1, p6, 6, "Pyramid6");

    float p8[8] = {-1.f, -0.5f, 0.f, 0.5f, 1.f, 0.5f, 0.f, -0.5f};
    WavetableContainer::getInstance().createLFOCoefficientsFromConstSections(
        2, p8, 8, "Pyramid8");

    float p12[12] = {-1.f, -0.666666666f, -0.33333333f, 0.f,
                     0.3333333f, 0.666666666f, 1.f, 0.6666666f,
                     0.33333333f, 0.f, -0.333333333f, -0.6666666f};
    WavetableContainer::getInstance().createLFOCoefficientsFromConstSections(
        3, p12, 12, "Pyramid12");

    float s3[3] = {-1.f, 0.f, 1.f};
    WavetableContainer::getInstance().createLFOCoefficientsFromConstSections(
        4, s3, 3, "Stair3");

    float s4[4] = {-1.f, -0.333333f, 0.333333f, 1.f};
    WavetableContainer::getInstance().createLFOCoefficientsFromConstSections(
        5, s4, 4, "Stair4");

    float s6[6] = {-1.f, -0.6f, -0.2f, 0.2f, 0.6f, 1.f};
    WavetableContainer::getInstance().createLFOCoefficientsFromConstSections(
        6, s6, 6, "Stair6");

    float s[12] = {0.f};

    for (int i = 0; i < 8; ++i)
    {
        s[i] = -1.f + i * (2.f / 7.f);
    }
    WavetableContainer::getInstance().createLFOCoefficientsFromConstSections(
        7, s, 8, "Stair8");
    for (int i = 0; i < 12; ++i)
    {
        s[i] = -1.f + i * (2.f / 11.f);
    }
    WavetableContainer::getInstance().createLFOCoefficientsFromConstSections(
        8, s, 12, "Stair12");
    float square[8] = {1.f, -1.f, -1.f, -1.f, -1.f, -1.f, -1.f, -1.f};
    WavetableContainer::getInstance().createLFOCoefficientsFromConstSections(
        9, square, 2, "Square50");
    WavetableContainer::getInstance().createLFOCoefficientsFromConstSections(
        10, square, 4, "Square25");
    WavetableContainer::getInstance().createLFOCoefficientsFromConstSections(
        11, square, 8, "Square12");
*/