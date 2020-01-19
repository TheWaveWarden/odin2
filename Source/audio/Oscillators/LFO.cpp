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

	setWavetablePointerNONCONST(16, WavetableContainer::getInstance().getWavedrawPointer(0));
	setWavetablePointerNONCONST(17, WavetableContainer::getInstance().getWavedrawPointer(1));
	setWavetablePointerNONCONST(18, WavetableContainer::getInstance().getWavedrawPointer(2));
}

float LFO::doOscillate() {
    jassert(m_samplerate > 0);

	if (!m_SH_active) {
		return doWavetable();
	} else {
		return doSampleHold();
	}
}

float LFO::doSampleHold() {
    jassert(m_samplerate > 0);

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