#include "ChiptuneArpeggiator.h"
#include "../../GlobalIncludes.h"
#include <cmath>

// ! bottleneck
float pitchShiftMultiplier(float p_semitones) {
	//0.05776226504 = ln(2)/12
	//apparently pow(a,b) is calculated as exp(ln(a)*b), hence this is faster
	return juce::dsp::FastMathApproximations::exp(0.05776226504 * p_semitones);
}

int ChiptuneArpeggiator::doArpeggiator() {
	if (!m_arp_active) {
		return 0;
	}

	if (*m_freq_mod) {
		m_inc = m_frequency * m_one_over_samplerate * pitchShiftMultiplier(*m_freq_mod * 24);
	} else {
		m_inc = m_frequency * m_one_over_samplerate;
	}

	// inc & wrap position inside step
	m_position_in_step += m_inc;
	while (m_position_in_step > STEP_LENGTH) {
		m_position_in_step -= STEP_LENGTH;
		++m_current_step;
		// inc & wrap step
		if (m_current_step > 2 || (!m_step_three_active && m_current_step > 1)) {
			m_current_step = 0;
		}
	}

	return m_step_semitones[m_current_step];
}
