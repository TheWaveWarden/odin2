#include "LFO.h"

void LFO::loadWavetables() {
	setWavetablePointer(0, m_WT_container->getWavetablePointers("Sine"));
	setWavetablePointer(1, m_WT_container->getWavetablePointers("Saw"));
	setWavetablePointer(2, m_WT_container->getWavetablePointers("Triangle"));

	setWavetablePointer(3, m_WT_container->getLFOPointers("Square50"));
	setWavetablePointer(4, m_WT_container->getLFOPointers("Square25"));
	setWavetablePointer(5, m_WT_container->getLFOPointers("Square12"));

	setWavetablePointer(6, m_WT_container->getLFOPointers("Spike"));

	setWavetablePointer(7, m_WT_container->getLFOPointers("Pyramid4"));
	setWavetablePointer(8, m_WT_container->getLFOPointers("Pyramid6"));
	setWavetablePointer(9, m_WT_container->getLFOPointers("Pyramid8"));
	setWavetablePointer(10, m_WT_container->getLFOPointers("Pyramid12"));

	setWavetablePointer(11, m_WT_container->getLFOPointers("Stair3"));
	setWavetablePointer(12, m_WT_container->getLFOPointers("Stair4"));
	setWavetablePointer(13, m_WT_container->getLFOPointers("Stair6"));
	setWavetablePointer(14, m_WT_container->getLFOPointers("Stair8"));
	setWavetablePointer(15, m_WT_container->getLFOPointers("Stair12"));

	setWavetablePointerNONCONST(16, m_WT_container->getWavedrawPointer(0));
	setWavetablePointerNONCONST(17, m_WT_container->getWavedrawPointer(1));
	setWavetablePointerNONCONST(18, m_WT_container->getWavedrawPointer(2));
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